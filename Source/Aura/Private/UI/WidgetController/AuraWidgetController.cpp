// CopyRight HenryChen


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"


void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

AAuraPlayerController* UAuraWidgetController::GetAuraPlayerController()
{
	if(AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if(AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
	if(AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if(AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;

}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
	{
		return;
	}
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AuraAbilitySystemComponent->GetAbilityTagBySpec(AbilitySpec));	
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagBySpec(AbilitySpec);
		Info.StatusTag = AuraAbilitySystemComponent->GetStatusTagBySpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetAuraAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}
