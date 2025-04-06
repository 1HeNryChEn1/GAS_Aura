// CopyRight HenryChen


#include "AbilitySystem/Abilities/DamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

float UDamageAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType) const
{
	checkf(DamageTypes.Contains(DamageType), TEXT("%s, %s"), *GetNameSafe(this), *DamageType.ToString());
	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
}

void UDamageAbility::CauseDamage(AActor* Target)
{
	auto DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (auto [Tag, ScalableFloat] : DamageTypes)
	{
		float ScaledDamage = ScalableFloat.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Tag, ScaledDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
}

FTaggedMontage UDamageAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.IsEmpty())
	{
		return {};
	}
	const auto Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
	return TaggedMontages[Selection];
}
