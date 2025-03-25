// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AuraAttributeSet.generated.h"

// These are Helper Macros
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define DEFINE_ONREP_FUNCTION(Attribute) \
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_##Attribute, Category = " Attributes") \
	FGameplayAttributeData Attribute; \
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Attribute); \
	UFUNCTION() \
	void OnRep_##Attribute(const FGameplayAttributeData& Old##Attribute) const{ \
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Attribute, Old##Attribute); \
	};

class ACharacter;
template<typename T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){};

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	FGameplayEffectContextHandle EffectContextHandle{};
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const ;

	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit);

	void SendXPEvent(const FEffectProperties& Props);
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	// Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);

	//Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")FGameplayAttributeData Health;ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);UFUNCTION()void OnRep_Health(const FGameplayAttributeData& OldHealth) const{GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);};
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes") FGameplayAttributeData Mana; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana); UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldMana) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana); };

	//Primary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes") FGameplayAttributeData Strength; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength); UFUNCTION() void OnRep_Strength(const FGameplayAttributeData& OldStrength) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes") FGameplayAttributeData Intelligence; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence); UFUNCTION() void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes") FGameplayAttributeData Resilience; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience); UFUNCTION() void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes") FGameplayAttributeData Vigor; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor); UFUNCTION() void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor); };

	//Secondary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes") FGameplayAttributeData Armor; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor); UFUNCTION() void OnRep_Armor(const FGameplayAttributeData& OldArmor) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes") FGameplayAttributeData ArmorPenetration; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration); UFUNCTION() void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes") FGameplayAttributeData BlockChance; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance); UFUNCTION() void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes") FGameplayAttributeData CriticalHitChance; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance); UFUNCTION() void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes") FGameplayAttributeData CriticalHitDamage; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage); UFUNCTION() void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes") FGameplayAttributeData CriticalHitResistance; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance); UFUNCTION() void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes") FGameplayAttributeData HealthRegeneration; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration); UFUNCTION() void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes") FGameplayAttributeData ManaRegeneration; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration); UFUNCTION() void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes") FGameplayAttributeData MaxHealth; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth); UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes") FGameplayAttributeData MaxMana; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana); UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attributes") FGameplayAttributeData FireResistance; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance); UFUNCTION() void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Resistance Attributes") FGameplayAttributeData ArcaneResistance; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance); UFUNCTION() void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Resistance Attributes") FGameplayAttributeData LightningResistance; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance); UFUNCTION() void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance); };
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes") FGameplayAttributeData PhysicalResistance; ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance); UFUNCTION() void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance); };

};
