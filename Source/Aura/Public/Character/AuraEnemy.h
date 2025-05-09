// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"
/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 EnemyLevel = 1;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
public:
	AAuraEnemy();

	//~ Begin HighLight Interface 
	virtual void HighlightActor_Implementation() override;

	virtual void UnHighlightActor_Implementation() override;

	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	//~ End HighLight Interface 

	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;

	virtual void Die(const FVector& DeathImpulse) override;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;

	virtual AActor* GetCombatTarget_Implementation() const override;
	//~ End Combat Interface 

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void PossessedBy(AController* NewController) override;

	void SetLevel(int32 InLevel) {EnemyLevel = InLevel;}
};
