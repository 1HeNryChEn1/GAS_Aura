// CopyRight HenryChen


#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

    FAuraGameplayTags::InitializeNativeGameplayTags();

}

UAuraAssetManager& UAuraAssetManager::Get()
{
    check(GEngine);
    UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
    check(AuraAssetManager);
    return *AuraAssetManager;
}
