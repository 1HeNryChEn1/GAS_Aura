// CopyRight HenryChen


#include "AbilitySystem/Data/AbilityInfo.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (auto& Info : AbilityInfos)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AbilityTag [%s] on AbilityInfo [%s]."), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return {};
}
