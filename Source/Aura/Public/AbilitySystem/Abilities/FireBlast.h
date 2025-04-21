// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DamageAbility.h"
#include "FireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UFireBlast : public UDamageAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	TSubclassOf<AAuraFireBall> FireBallClass;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;

public:
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
};
