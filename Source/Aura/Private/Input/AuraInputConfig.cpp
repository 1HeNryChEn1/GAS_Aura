// CopyRight HenryChen


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (auto& InputAction : AbilityInputActions)
	{
		if (InputAction.InputAction && InputTag.MatchesTagExact(InputAction.InputTag)/* or InputAction.InputTag.MatchesTagExact(InputTag) */)
		{
			return InputAction.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this))
	}
	return nullptr;
}
