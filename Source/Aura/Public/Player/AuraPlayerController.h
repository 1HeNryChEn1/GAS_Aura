// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
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
	UPROPERTY (EditAnywhere, Category = "GAS|Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY (EditAnywhere, Category = "GAS|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY (EditAnywhere, Category = "GAS|Input")
	TObjectPtr<UInputAction> ShiftAction;

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	/** Click to move **/
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime;
	float ShortPressThreshold;
	bool bAutoRunning;
	bool bTargeting;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius;

	bool bShiftKeyDown = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	/** End Click to move **/

	void ShiftPressed() {bShiftKeyDown = true;}

	void ShiftReleased() {bShiftKeyDown = false;}

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);

	void AbilityInputTagHeld(FGameplayTag InputTag);

	void AutoRunning();

	UAuraAbilitySystemComponent* GetASC();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

};

