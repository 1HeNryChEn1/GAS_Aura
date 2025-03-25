// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"


UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY (EditAnywhere, Category = "SpringArm")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY (EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	virtual void InitAbilityActorInfo() override;

public:
	AAuraCharacter();

	// Begin Combat Interface 
	virtual int32 GetPlayerLevel_Implementation() override;
	// End Combat Interface

	// Begin Player Interface
	virtual void AddToXP_Implementation(int32 InXP) override;

	virtual void LevelUp_Implementation() override;

	virtual int32 GetXP_Implementation() override;

	virtual int32 FindLevelByXP_Implementation(int32 InXP) const override;

	virtual int32 GetAttributePointsReward_Implementation(int32 InLevel) const override;

	virtual int32 GetSpellPointsReward_Implementation(int32 InLevel) const override;

	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;

	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;

	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

	// End Player Interface

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
};
