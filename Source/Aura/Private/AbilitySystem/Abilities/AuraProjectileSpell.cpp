// CopyRight HenryChen


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Game/ObjectPoolSubsystem.h"
#include "GameFramework/Pawn.h"
#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	if(!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	// Set Projectile Spell Rotation
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	/*
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	*/
	UObjectPoolSubsystem* PoolSubsystem = GetAvatarActorFromActorInfo()->GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>();
	if(PoolSubsystem)
	{
		AAuraProjectile* Projectile = Cast<AAuraProjectile>(PoolSubsystem->GetPooledObject(ProjectileClass));
		if(!Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get pooled object of type %s"), *ProjectileClass->GetName());
			return;
		}
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		IPoolableInterface::Execute_Activate(Projectile, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()));
	}
	// Projectile->FinishSpawning(SpawnTransform);
}
