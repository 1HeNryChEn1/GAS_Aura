// CopyRight HenryChen


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	FollowTime = 0.f;
	ShortPressThreshold = 0.5f;
	bAutoRunning = false;
	AutoRunAcceptanceRadius = 10.f;
}

void AAuraPlayerController::BeginPlay()
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

	AutoRunning();

	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if(!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if(DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if(IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if(IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::AutoRunning()
{
	if(!bAutoRunning)
	{
		return;
	}
	if(auto ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		auto DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if(!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = ThisActor;
	if(IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}
	/**
	 * A. Last is null && This is null : do nothing
	 * B. Last is null && This is valid : highlight This
	 * C. Last is Valid && This is null : unhighlight Last
	 * D. Last is Valid && This is Valid &&
	 *						D1. Last != This : unhighlight Last, highlight This
	 *						D2.	Last == This : do nothing
	 */
	if(LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}
	if(GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if(GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	if(TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		const APawn* ControllerPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if(IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
			{
				IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			}
			else if(GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
			if(const auto NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControllerPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for(auto& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if(!NavPath->PathPoints.IsEmpty())
				{
					CachedDestination = NavPath->PathPoints.Last();
				}
				bAutoRunning = true;
			}
			if(GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
		}
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if(TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if(CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if(auto* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if(IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	// Base Action
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	// Ability Action
	AuraInputComponent->BindAbilityActions(InputConfig, this,
										   &AAuraPlayerController::AbilityInputTagPressed,
										   &AAuraPlayerController::AbilityInputTagReleased,
										   &AAuraPlayerController::AbilityInputTagHeld);

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	FRotator Rotation = GetControlRotation();
	Rotation.Roll = 0.f;
	Rotation.Pitch = 0.f;

	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if(const auto ControlledPawn = GetPawn<APawn>()){
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
		bAutoRunning = false;
	}
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if(IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if(IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}