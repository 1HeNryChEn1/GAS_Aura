// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

USTRUCT()
struct AURA_API FPooledObjectArray
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<AActor>> Objects;
};

UCLASS()
class AURA_API UObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

    UPROPERTY()
    TMap<TSubclassOf<AActor>, FPooledObjectArray> AvailableObjects;

    //  Number of actors to spawn when expanding dynamically
    static constexpr int32 ExpansionSize = 5;

public:
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void InitializePool(TSubclassOf<AActor> ObjectClass, int32 PoolSize);

    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    AActor* GetPooledObject(TSubclassOf<AActor> ObjectClass);

    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void ReturnPooledObject(AActor* Object);

    /** Clean up and destroy all pooled actors */
    virtual void Deinitialize() override;
};

