// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerLevel)
	int32 PlayerLevel = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, Replicated = OnRep_SpellPoints)
	int32 SpellPoints = 1;

	UPROPERTY (VisibleAnywhere, Replicated = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldPlayerLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

public:
	FOnPlayerStatChanged OnXPChangedDelegate;

	FOnPlayerStatChanged OnLevelChangedDelegate;

	FOnPlayerStatChanged OnAttributePointsChangedDelegate;

	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const {return PlayerLevel;}

	FORCEINLINE int32 GetXP() const {return XP;}

	FORCEINLINE int32 GetSpellPoints () const {return SpellPoints;}

	FORCEINLINE int32 GetAttributePoints () const {return AttributePoints;}

	void AddToXP(int32 InXP);

	void AddToLevel(int32 InLevel);

	void AddToAttributePoints(int32 InAttributePoints);

	void AddToSpellPoints(int32 InSpellPoints);

	void SetXP(int32 InXP);

	void SetLevel(int32 InLevel);

	void SetAttributePoints(int32 InAttributePoints);

	void SetSpellPoints(int32 InSpellPoints);
};

