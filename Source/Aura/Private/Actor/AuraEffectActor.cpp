// CopyRight HenryChen


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	 if(!TargetActor)
		 return;
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovalPolicy::RemoveOnEndOverLap)  // do not store the handle unless policy is Effect-Removable
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstanceEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverLap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverLap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverLap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstanceEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverLap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverLap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverLap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovePolicy == EEffectRemovalPolicy::RemoveOnEndOverLap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (IsValid(TargetASC))
		{
			return;
		}
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto [HandleKey, HandleValue] : ActiveEffectHandles)
		{
			if (TargetASC == HandleValue)
			{
				TargetASC->RemoveActiveGameplayEffect(HandleKey);
				// Store the key first instead of erasing it during the loop, which will break the validation of iterator.
				HandlesToRemove.Add(HandleKey);
			}
		}
		for (auto& HandleToRemove : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(HandleToRemove);
		}
	}
}
