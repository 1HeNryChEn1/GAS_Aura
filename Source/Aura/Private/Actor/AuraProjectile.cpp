// CopyRight HenryChen


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Game/ObjectPoolSubsystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::Activate_Implementation(const FTransform& SpawnTransform, AActor* InOwner, APawn* InInstigator)
{
	SetActorTransform(SpawnTransform);
	SetOwner(InOwner);
	SetInstigator(InInstigator);


	if(ProjectileMovement)
	{
		FVector Direction = SpawnTransform.GetRotation().Rotator().Vector();
		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	}
	bHit = false;

	GetWorld()->GetTimerManager().SetTimer(
		CollisionTimerHandle,
		this,
		&AAuraProjectile::EnableCollision,
		0.15f,
		false
	);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());

	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
}

void AAuraProjectile::Deactivate_Implementation()
{
	// Disable rendering, collision, and tick
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	if(ProjectileMovement)
	{
		ProjectileMovement->Velocity = FVector::ZeroVector;
		ProjectileMovement->HomingTargetComponent = nullptr;
		ProjectileMovement->StopMovementImmediately(); // Stop any movement
	}

	if(LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if(!bHit && !HasAuthority()) //server size had got hit, so client must play
	{
		OnHit();
	}
}

void AAuraProjectile::Reset_Implementation()
{
	if(HomingTargetSceneComponent)
	{
		HomingTargetSceneComponent->SetWorldLocation(FVector::ZeroVector);
	}
	ProjectileMovement->HomingAccelerationMagnitude = 0.f;
	ProjectileMovement->bIsHomingProjectile = false;
}

void AAuraProjectile::EnableCollision()
{
	Sphere->IgnoreActorWhenMoving(GetOwner(), true);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetActorEnableCollision(true);
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	SetReplicateMovement(true);
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if(LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	if(LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if(!bHit && !HasAuthority()) //server size had got hit, so client must play
	{
		OnHit();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor))
	{
		return;
	}
	if(!bHit)
	{
		OnHit();
	}
	if(HasAuthority())
	{
		// Damage Effect
		if(auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Impulse
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			// Knockback 
			if(FMath::RandRange(0, 99) < DamageEffectParams.KnockbackChance)
			{
				FRotator Rotation = GetActorRotation();
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		// Play Hit effect
		Execute_Deactivate(this);
		UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>();
		if(PoolSubsystem)
		{
			PoolSubsystem->ReturnPooledObject(this);
		}
	}
	else
	{
		bHit = true;
	}
}

bool AAuraProjectile::IsValidOverlap(AActor* OtherActor)
{
	if(DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if(SourceAvatarActor == OtherActor) return false;
	if(UAuraAbilitySystemLibrary::IsFriend(SourceAvatarActor, OtherActor)) return false;

	return true;
}