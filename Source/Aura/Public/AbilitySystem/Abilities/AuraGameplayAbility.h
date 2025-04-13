// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCoolDown(float InLevel = 1.f) const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level);

	virtual FString GetNextLevelDescription(int32 Level);

	static FString GetLockedDescription(int32 Level);
};
