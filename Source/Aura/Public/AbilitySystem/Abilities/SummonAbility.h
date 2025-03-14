// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "SummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API USummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:

public:
	UPROPERTY(EditDefaultsOnly, category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, category = "Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, category = "Summoning")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, category = "Summoning")
	float SpawnSpread = 90.f;

	UFUNCTION(BlueprintPure, category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation();
};
