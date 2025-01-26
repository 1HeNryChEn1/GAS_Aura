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

#define DEFINE_ONREP_FUNCTION(AttributeType , Attribute) \
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_##Attribute, Category = TEXT(#AttributeType " Attributes")) \
	FGameplayAttributeData Attribute; \
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Attribute); \
	UFUNCTION() \
	void OnRep_##Attribute(const FGameplayAttributeData& Old##Attribute) const{ \
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Attribute, Old##Attribute); \
	};

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

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	DEFINE_ONREP_FUNCTION(Vital, Health);
	DEFINE_ONREP_FUNCTION(Vital, Mana);

	DEFINE_ONREP_FUNCTION(Primary, Strength);
	DEFINE_ONREP_FUNCTION(Primary, Intelligence);
	DEFINE_ONREP_FUNCTION(Primary, Resilience);
	DEFINE_ONREP_FUNCTION(Primary, Vigor);

	DEFINE_ONREP_FUNCTION(Secondary, Armor);
	DEFINE_ONREP_FUNCTION(Secondary, ArmorPenetration);
	DEFINE_ONREP_FUNCTION(Secondary, BlockChance);
	DEFINE_ONREP_FUNCTION(Secondary, CriticalHitChance);
	DEFINE_ONREP_FUNCTION(Secondary, CriticalHitDamage);
	DEFINE_ONREP_FUNCTION(Secondary, CriticalHitResistance);
	DEFINE_ONREP_FUNCTION(Secondary, HealthRegeneration);
	DEFINE_ONREP_FUNCTION(Secondary, ManaRegeneration);
	DEFINE_ONREP_FUNCTION(Secondary, MaxHealth);
	DEFINE_ONREP_FUNCTION(Secondary, MaxMana);
};
