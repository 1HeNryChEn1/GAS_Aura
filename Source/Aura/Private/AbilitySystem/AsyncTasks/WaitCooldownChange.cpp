// CopyRight HenryChen


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag IncomingCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0);
		EndTask ();
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTags, GrantedTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	SpecApplied.GetAllGrantedTags(GrantedTags);
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact((CooldownTag)))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{
			float HighestTime = TimesRemaining[0];
			for (const float x : TimesRemaining)
			{
				HighestTime = FMath::Max(HighestTime, x);
			}
			float TimeRemaining = HighestTime;
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange= NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;
	if (!IsValid (AbilitySystemComponent) || !InCooldownTag.IsValid ())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	// To know when a cooldown has ended (Cooldown Tag has been removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag, 
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChanged);

	// To know when a cooldown effect has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCooldownChange,
		&UWaitCooldownChange::OnActiveEffectAdded
	);
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (IsValid (ASC) && CooldownTag.IsValid ())
	{
		ASC->RegisterGameplayTagEvent(
			CooldownTag,
			EGameplayTagEventType::NewOrRemoved
		).RemoveAll(this);
	}
	SetReadyToDestroy();
}
