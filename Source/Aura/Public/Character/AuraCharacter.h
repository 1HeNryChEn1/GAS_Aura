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

protected:

public:
	AAuraCharacter();
};
