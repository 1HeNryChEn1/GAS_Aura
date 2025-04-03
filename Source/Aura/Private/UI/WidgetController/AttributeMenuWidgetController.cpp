// CopyRight HenryChen


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"


void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
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
	check(AttributeInfo);
	for(auto& [Tag, AttributeFunctPtr] : GetAuraAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeFunctPtr()).AddLambda(
			[this, Tag, AttributeFunctPtr](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag, AttributeFunctPtr());
			});
	}

	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddUObject(this, &UAttributeMenuWidgetController::OnAttributePointsChanged);
	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddUObject(this, &UAttributeMenuWidgetController::OnSpellPointsChanged);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for(auto& [Tag, AttributeFunctPtr] : GetAuraAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Tag, AttributeFunctPtr());
	}

	AttributePointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetAttributePoints());
}

void UAttributeMenuWidgetController::OnAttributePointsChanged(int NewAttributePoints)
{
	AttributePointsChangedDelegate.Broadcast(NewAttributePoints);
}

void UAttributeMenuWidgetController::OnSpellPointsChanged(int NewSpellPoints)
{
	SpellPointsChangedDelegate.Broadcast(NewSpellPoints);
}