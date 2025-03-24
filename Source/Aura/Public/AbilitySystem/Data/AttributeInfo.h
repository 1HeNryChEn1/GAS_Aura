// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription{};

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};


/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInfos;

	FAuraAttributeInfo FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

};
