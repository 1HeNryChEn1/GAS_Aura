// CopyRight HenryChen


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			if (auto OverlayWidgetController = AuraHUD->GetOverlayWidgetController())
			{
				return OverlayWidgetController;
			}
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			if (auto AttributeMenuWidgetController = AuraHUD->GetAttributeMenuWidgetController())
			{
				return AttributeMenuWidgetController;
			}
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

FVector2D UAuraAbilitySystemLibrary::GetRandomPosition()
{
	FVector2D ScreenSize;
	GEngine->GameViewport->GetViewportSize(ScreenSize);
	FVector2D ScreenCenter = ScreenSize * 0.5f;
	float OffsetRange = 50.0f;
	float RandomOffsetX = FMath::RandRange(-OffsetRange, OffsetRange);
	float RandomOffsetY = FMath::RandRange(-OffsetRange, OffsetRange);
	return ScreenCenter + FVector2D(RandomOffsetX, RandomOffsetY);
}
