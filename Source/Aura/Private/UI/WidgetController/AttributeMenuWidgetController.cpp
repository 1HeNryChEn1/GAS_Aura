// CopyRight HenryChen


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"


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
	for (auto& [Tag, AttributeFunctPtr] : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeFunctPtr()).AddLambda(
		[this, Tag, AttributeFunctPtr, AuraAttributeSet](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Tag, AttributeFunctPtr());
		});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& [Tag, AttributeFunctPtr] : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Tag, AttributeFunctPtr());
	}
}