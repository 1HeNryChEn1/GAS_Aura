// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "DamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
};
