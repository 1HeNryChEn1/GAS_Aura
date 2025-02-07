// CopyRight HenryChen


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController ()
{
	bReplicates = true;

}

void AAuraPlayerController::BeginPlay ()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); 
	if(SubSystem)
	{
		SubSystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	/**
	 * A. Last is null && This is null : do nothing
	 * B. Last is null && This is valid : highlight This
	 * C. Last is Valid && This is null : unhighlight Last
	 * D. Last is Valid && This is Valid &&
	 *						D1. Last != This : unhighlight Last, highlight This
	 *						D2.	Last == This : do nothing
	 */
	if (!LastActor)
	{
		if (!ThisActor)
		{
			// A
			//Do nothing
		}
		else
		{
			// B
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (!ThisActor)
		{
			// C
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				// D1
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			// D2
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
	GetASC()->AbilityInputTagHeld(InputTag);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}


void AAuraPlayerController::SetupInputComponent ()
{
	Super::SetupInputComponent();

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	// Base Action
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	// Ability Action
	AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
}

void AAuraPlayerController::Move (const FInputActionValue &InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	FRotator Rotation = GetControlRotation();
	Rotation.Roll = 0.f;
	Rotation.Pitch = 0.f;

	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if(auto ControlledPawn = GetPawn<APawn>()){
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}