// CopyRight HenryChen


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"


void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
															const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for(auto& [Tag, AttributeFunctPtr] : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeFunctPtr()).AddLambda(
			[this, Tag, AttributeFunctPtr, AuraAttributeSet](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag, AttributeFunctPtr());
			});
	}

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnAttributePointsChangedDelegate.AddUObject(this, &UAttributeMenuWidgetController::OnAttributePointsChanged);
	AuraPlayerState->OnSpellPointsChangedDelegate.AddUObject(this, &UAttributeMenuWidgetController::OnSpellPointsChanged);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for(auto& [Tag, AttributeFunctPtr] : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Tag, AttributeFunctPtr());
	}

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::OnAttributePointsChanged(int NewAttributePoints)
{
	AttributePointsChangedDelegate.Broadcast(NewAttributePoints);
}

void UAttributeMenuWidgetController::OnSpellPointsChanged(int NewSpellPoints)
{
	SpellPointsChangedDelegate.Broadcast(NewSpellPoints);
}