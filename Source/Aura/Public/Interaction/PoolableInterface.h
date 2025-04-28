// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPoolableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Object Pool")
    void Activate(const FTransform& SpawnTransform, AActor* Owner, APawn* Instigator);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Object Pool")
    void Deactivate();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Object Pool")
    void Reset();
};
