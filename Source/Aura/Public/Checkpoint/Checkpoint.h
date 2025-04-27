// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;
/**
 *
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface, public  IHighlightInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();

	/* Highlight Interface */
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/* Highlight Interface */
public:
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;

	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	// Begin SaveInterface
	virtual bool ShouldLoadTransform_Implementation() override { return false; };

	virtual void LoadActor_Implementation() override;
	// End SaveInterface 


};
