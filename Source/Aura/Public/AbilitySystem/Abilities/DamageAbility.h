// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "DamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType) const;
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* Target);

	UFUNCTION(Blueprintpure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);
};
