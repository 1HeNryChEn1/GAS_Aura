// CopyRight HenryChen


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

// could be static library
FVector2D UAuraWidgetController::GetRandomPosition()
{
	FVector2D ScreenSize;
	GEngine->GameViewport->GetViewportSize(ScreenSize);
	FVector2D ScreenCenter = ScreenSize * 0.5f;
	float OffsetRange = 50.0f;
	float RandomOffsetX = FMath::RandRange(-OffsetRange, OffsetRange);
	float RandomOffsetY = FMath::RandRange(-OffsetRange, OffsetRange);
	return ScreenCenter + FVector2D(RandomOffsetX, RandomOffsetY);
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
