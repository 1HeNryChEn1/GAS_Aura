// CopyRight HenryChen


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPlayerState()->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnPlayerLevelChanged);
	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnAttributePointsChanged);
	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnSpellPointsChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
    GetAuraAttributeSet()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
	    OnHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	    GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
	    OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	    GetAuraAttributeSet()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
	    OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	    GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
	    OnMaxManaChanged.Broadcast(Data.NewValue);
	});

	if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)  // When callback binding is slower than the initialization of Abilities.
	{
		BroadcastInitialValues();
	}
	else
	{
		GetAuraAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
	}

	GetAuraAbilitySystemComponent()->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const auto &Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDateTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);  // could be nullptr
					if (Row)
					{
						MessageWidgetRow.Broadcast(*Row);
					}
				}

			}
		}
	);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	TObjectPtr<ULevelUpInfo> LevelUpInfo = GetAuraPlayerState()->LevelUpInfo;
	check(LevelUpInfo);

	const int32 CurrentLevel = LevelUpInfo->FindLevelByXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInfos.Num() - 1;
	if (0 < CurrentLevel && CurrentLevel <= MaxLevel)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInfos[CurrentLevel].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInfos[CurrentLevel - 1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		const float NewXPPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(NewXPPercent);
	}	
}

void UOverlayWidgetController::OnPlayerLevelChanged(int32 NewPlayerLevel) const
{
	OnPlayerLevelChangedDelegate.Broadcast(NewPlayerLevel);
}

void UOverlayWidgetController::OnAttributePointsChanged(int32 NewAttributePoints) const
{
	OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
}

void UOverlayWidgetController::OnSpellPointsChanged(int32 NewSpellPoints) const
{
	OnSpellPointsChangedDelegate.Broadcast(NewSpellPoints);
}
