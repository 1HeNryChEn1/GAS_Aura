// CopyRight HenryChen


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 7999.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->BrakingDecelerationWalking = 8000.f;
	GetCharacterMovement()->GroundFriction = 8.0f;
	GetCharacterMovement()->MaxAcceleration = 10000.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f;

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	TopDownCamera->SetupAttachment(SpringArm);
	TopDownCamera->bUsePawnControlRotation = false;

	CharacterClass = ECharacterClass::Elementalist;
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
	{
		AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (AuraGM)
		{
			AuraGM->PlayerDied(this);
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);
	TopDownCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AAuraCharacter::GetXP_Implementation()
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelByXP_Implementation(int32 InXP) const
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelByXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 InLevel) const
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfos[InLevel].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 InLevel) const
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfos[InLevel].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerLevel);

	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(!IsValid(AuraGameMode))
	{
		return;
	}

	ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
	if(SaveData == nullptr) return;

	SaveData->PlayerStartTag = CheckpointTag;

	if(AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
	{
		SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
		SaveData->XP = AuraPlayerState->GetXP();
		SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
		SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
	}
	SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

	SaveData->bFirstTimeLoadIn = false;

	if(!HasAuthority()) return;

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	FForEachAbility SaveAbilityDelegate;
	SaveData->SavedAbilities.Empty();
	SaveAbilityDelegate.BindLambda([this, AuraASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
								   {
									   const FGameplayTag AbilityTag = AuraASC->GetAbilityTagBySpec(AbilitySpec);
									   UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
									   FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);

									   FSavedAbility SavedAbility;
									   SavedAbility.GameplayAbility = Info.Ability;
									   SavedAbility.AbilityLevel = AbilitySpec.Level;
									   SavedAbility.AbilitySlot = AuraASC->GetSlotFromAbilityTag(AbilityTag);
									   SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
									   SavedAbility.AbilityTag = AbilityTag;
									   SavedAbility.AbilityType = Info.AbilityType;

									   SaveData->SavedAbilities.AddUnique(SavedAbility);
								   });
	AuraASC->ForEachAbility(SaveAbilityDelegate);

	AuraGameMode->SaveInGameProgressData(SaveData);
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init AbilityActorInfo for the Server
	InitAbilityActorInfo();
	LoadProgress();

	if(AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGameMode->LoadWorldState(GetWorld());
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init AbilityActorInfo for the Client
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_Stunned()
{
	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if(bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if(bIsStunned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void AAuraCharacter::InitAbilityActorInfo()
{
	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(const auto AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation()
{
	if(IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCamera->GetComponentLocation();
		const FVector NiagaraLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate();
	}
}

void AAuraCharacter::LoadProgress()
{
	if(AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;

		if(SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}

			if(AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}

			UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}