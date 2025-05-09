// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"


UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "SpringArm")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> TopDownCamera;

	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;

	FTimerHandle DeathTimer;

	AAuraCharacter();

	// Begin Combat Interface 
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	// End Combat Interface

	// Begin Player Interface
	virtual void AddToXP_Implementation(int32 InXP) override;

	virtual void LevelUp_Implementation() override;

	virtual int32 GetXP_Implementation() override;

	virtual int32 FindLevelByXP_Implementation(int32 InXP) const override;

	virtual int32 GetAttributePointsReward_Implementation(int32 InLevel) const override;

	virtual int32 GetSpellPointsReward_Implementation(int32 InLevel) const override;

	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;

	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;

	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

	virtual int32 GetAttributePoints_Implementation() const override;

	virtual int32 GetSpellPoints_Implementation() const override;

	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;

	virtual void HideMagicCircle_Implementation() override;

	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	// End Player Interface

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void OnRep_Stunned() override;

	virtual void OnRep_Burned() override;

	void LoadProgress();
};
