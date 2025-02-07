// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	
	UPROPERTY (EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY (EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);

	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetASC();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

};

