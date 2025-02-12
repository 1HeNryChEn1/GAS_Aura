// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"


UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

	UPROPERTY (EditAnywhere, Category = "SpringArm")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY (EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	virtual void InitAbilityActorInfo() override;

protected:

public:
	AAuraCharacter();

	//~ Begin Combat Interface 
	virtual int32 GetPlayerLevel() override;
	//~ End Combat Interface

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
};
