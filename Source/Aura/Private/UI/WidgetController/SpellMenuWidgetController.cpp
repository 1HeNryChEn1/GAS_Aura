// CopyRight HenryChen


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewAbilityLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bShouldEnableSpellPointsButton = false;
			bool bShouldEnableEquipButton = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bShouldEnableSpellPointsButton, bShouldEnableEquipButton);
			FString DescriptionString;
			FString NextLevelDescriptionString;
			GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, DescriptionString, NextLevelDescriptionString);
			SpellGlobeSelectedDelegate.Broadcast(
				bShouldEnableSpellPointsButton, 
				bShouldEnableEquipButton,
				DescriptionString,
				NextLevelDescriptionString);
		}
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	 {
		 SpellPointsChangedDelegate.Broadcast(SpellPoints);
		 CurrentSpellPoints = SpellPoints;

		 bool bShouldEnableSpellPointsButton = false;
		 bool bShouldEnableEquipButton = false;
		 ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bShouldEnableSpellPointsButton, bShouldEnableEquipButton);
		FString DescriptionString;
		FString NextLevelDescriptionString;
		GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, DescriptionString, NextLevelDescriptionString);
		 SpellGlobeSelectedDelegate.Broadcast(
			 bShouldEnableSpellPointsButton,
			 bShouldEnableEquipButton,
			 DescriptionString,
			 NextLevelDescriptionString);
	 });

	GetAuraAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
}

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(AbilityType);
		bWaitingForEquipSelection = false;
	}

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNode = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNode || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_None;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusTagBySpec(*AbilitySpec);
	}
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bShouldEnableSpellPointsButton = false;
	bool bShouldEnableEquipButton = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bShouldEnableSpellPointsButton, bShouldEnableEquipButton);
	FString DescriptionString;
	FString NextLevelDescriptionString;
	GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, DescriptionString, NextLevelDescriptionString);
	SpellGlobeSelectedDelegate.Broadcast(
		bShouldEnableSpellPointsButton, 
		bShouldEnableEquipButton,
		DescriptionString,
		NextLevelDescriptionString);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
	bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;

	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
		bShouldEnableEquipButton = true;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
		bShouldEnableEquipButton = true;
	}
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(AbilityType);
		bWaitingForEquipSelection = false;
	}
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(
		false,
		false,
		FString(),
		FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;


	const FGameplayTag SelectedStatus = GetAuraAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbility.Ability);

	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection)
	{
		return;
	}
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType))
	{
		return;
	}

	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);

}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}
