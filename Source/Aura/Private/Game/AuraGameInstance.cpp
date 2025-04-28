// CopyRight HenryChen


#include "Game/AuraGameInstance.h"

#include "Actor/AuraProjectile.h"
#include "Game/ObjectPoolSubsystem.h"

void UAuraGameInstance::Init()
{
	Super::Init();

    UObjectPoolSubsystem* PoolSubsystem = GetSubsystem<UObjectPoolSubsystem>();
    if (PoolSubsystem)
    {
        PoolSubsystem->InitializePool(AAuraProjectile::StaticClass(), 30);
    }
}
