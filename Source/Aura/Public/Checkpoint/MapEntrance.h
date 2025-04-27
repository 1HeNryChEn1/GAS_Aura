// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult) override;

public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	/* HighlightInterface */
	virtual void HighlightActor_Implementation() override;
	/* End HighlightInterface */

	/* SaveInterface */
	virtual void LoadActor_Implementation() override;
	/* End SaveInterface */

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
};
