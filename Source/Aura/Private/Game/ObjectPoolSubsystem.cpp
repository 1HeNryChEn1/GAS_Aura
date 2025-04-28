// CopyRight HenryChen

#include "Game/ObjectPoolSubsystem.h"

void UObjectPoolSubsystem::InitializePool(TSubclassOf<AActor> ObjectClass, int32 PoolSize)
{
	if(!ObjectClass)
	{
		return;
	}

	TArray<TObjectPtr<AActor>>& Pool = AvailableObjects.FindOrAdd(ObjectClass).Objects;
	// Pre-allocate memory to avoid reallocations

	Pool.Reserve(PoolSize);

	for(int32 i = 0; i < PoolSize; i++)
	{
		AActor* NewObject = GetWorld()->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if(!IsValid(NewObject))
		{
			continue;
		}

		// Disable rendering, collision, and tick. External systems should also perform this step to increase robustness.
		NewObject->SetActorHiddenInGame(true);
		NewObject->SetActorEnableCollision(false);

		Pool.Add(NewObject);
	}
}

AActor* UObjectPoolSubsystem::GetPooledObject(TSubclassOf<AActor> ObjectClass)
{
	if(!ObjectClass)
	{
		return nullptr;
	}

	FPooledObjectArray* PoolPtr = AvailableObjects.Find(ObjectClass);
	if(PoolPtr == nullptr)
	{
		TArray<TObjectPtr<AActor>>& Pool = PoolPtr->Objects;
		if(Pool.Num() > 0)
		{
			AActor* Object = Pool.Pop(false); // Pop without Shrinking
			Object->SetActorHiddenInGame(false);
			Object->SetActorEnableCollision(true);
			return Object;
		}
	}

	// Dynamic expanding ?
	AActor* NewObject = GetWorld()->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
	return NewObject;
}

void UObjectPoolSubsystem::ReturnPooledObject(AActor* Object)
{
	if(!IsValid(Object))
	{
		return;
	}

	Object->SetActorHiddenInGame(true);
	Object->SetActorEnableCollision(false);

	TSubclassOf<AActor> ObjectClass = Object->GetClass();
	AvailableObjects.FindOrAdd(ObjectClass).Objects.Add(Object);
}

void UObjectPoolSubsystem::Deinitialize()
{
	// Destroy all actors created by the pool
    for (auto& [TypeOfClass, Pool] : AvailableObjects)
    {
        for (TObjectPtr<AActor> ActorPtr : Pool.Objects)
        {
            if (IsValid(ActorPtr))
            {
                ActorPtr->Destroy();
            }
        }
    }
    AvailableObjects.Empty();
}