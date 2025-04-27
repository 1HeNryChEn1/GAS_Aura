// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "UI/Widgets/DamageTextComponent.h"
#include "AuraPlayerController.generated.h"

class IHighlightInterface;
class AMagicCircle;
class UNiagaraSystem;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "GAS|Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "GAS|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "GAS|Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	UPROPERTY()
	TObjectPtr<AActor> LastActor;

	UPROPERTY()
	TObjectPtr<AActor> ThisActor;

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
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;
	bool bShiftKeyDown = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	/** End Click to move **/

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle; 

	void ShiftPressed() {
		bShiftKeyDown = true;
	}

	void ShiftReleased() {
		bShiftKeyDown = false;
	}

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);

	void AbilityInputTagHeld(FGameplayTag InputTag);

	void AutoRunning();

	UAuraAbilitySystemComponent* GetASC();

	void UpdateMagicCircleLocation();

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void SetAutoRunning(const bool bAutoRun) { bAutoRunning = bAutoRun; }

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
};