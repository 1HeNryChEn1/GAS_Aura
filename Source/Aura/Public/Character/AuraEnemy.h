// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 PlayerLevel = 1;

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

public:
	AAuraEnemy();
	//~ Begin Enemy Interface 
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End Enemy Interface 
	
	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel() override;

	//~ End Combat Interface 
};
