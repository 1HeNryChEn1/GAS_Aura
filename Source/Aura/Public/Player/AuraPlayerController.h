// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY (EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;
};
