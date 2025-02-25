// CopyRight HenryChen
#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
private:

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
public:
	bool IsCriticalHit() const
	{
		return bIsCriticalHit;
	}

	bool IsBlockedHit() const
	{
		return bIsBlockedHit;
	}

	void SetIsCriticalHit(const bool bInIsCriticalHit){bIsCriticalHit = bInIsCriticalHit;}

	void SetIsBlockedHit(const bool bInIsBlockedHit){bIsBlockedHit = bInIsBlockedHit;}

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FAuraGameplayEffectContext* Duplicate() const override;

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
