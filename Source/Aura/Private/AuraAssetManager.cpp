// CopyRight HenryChen


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

    FAuraGameplayTags::InitializeNativeGameplayTags();
    UAbilitySystemGlobals::Get().InitGlobalData();
}

UAuraAssetManager& UAuraAssetManager::Get()
{
    check(GEngine);
    UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
    check(AuraAssetManager);
    return *AuraAssetManager;
}
