// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrengthChangedSignature, float, NewStrength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntelligenceChangedSignature, float, NewIntelligence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResilienceChangedSignature, float, NewResilience);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVigorChangedSignature, float, NewVigor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorChangedSignature, float, NewArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorPenetrationChangedSignature, float, NewArmorPenetration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockChanceChangedSignature, float, NewBlockChance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalHitChanceChangedSignature, float, NewCriticalHitChance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalHitDamageChangedSignature, float, NewCriticalHitDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalHitResistanceChangedSignature, float, NewCriticalHitResistance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthRegenerationChangedSignature, float, NewHealthRegeneration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaRegenerationChangedSignature, float, NewManaRegeneration);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnStrengthChangedSignature OnStrengthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnIntelligenceChangedSignature OnIntelligenceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnResilienceChangedSignature OnResilienceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnVigorChangedSignature OnVigorChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnArmorChangedSignature OnArmorChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnArmorPenetrationChangedSignature OnArmorPenetrationChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnBlockChanceChangedSignature OnBlockChanceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCriticalHitChanceChangedSignature OnCriticalHitChanceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCriticalHitDamageChangedSignature OnCriticalHitDamageChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCriticalHitResistanceChangedSignature OnCriticalHitResistanceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealthRegenerationChangedSignature OnHealthRegenerationChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnManaRegenerationChangedSignature OnManaRegenerationChanged;

public:
	virtual void BindCallbacksToDependencies() override;

	virtual void BroadcastInitialValues() override;
};
