// Copyright ANNIXA.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Aura/Interaction/CombatInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	
protected:
	virtual void BeginPlay() override;
	
public:
	// ~Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~End IAbilitySystemInterface
	
	// ~Begin ICombatInterface
	virtual FVector GetCombatSocketLocation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	// ~End ICombatInterface
	
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
protected:
	/** 初始化 Ability Actor Info ( GAS ) */
	virtual void InitAbilityActorInfo();
	
	// 对角色自身应用 GE
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	
	/** 初始化默认属性 */
	virtual void InitializeDefaultAttributes() const;
	
	/** 为角色添加能力 */
	void AddCharacterAbilities() const; 
	
protected:
	// 角色手持的武器
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	// 这里在基类存储一份 AbilitySystemComponent 和 AttributeSet 但是不在基类中构造
	// 玩家会在 PlayerState 中构造，敌人 AI 会在敌人类中构造
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// 该 GE 用来初始化 Primary 属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesEffect;
	
	// 该 GE 用来初始化 Secondary 属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffect;
	
	// 该 GE 用来初始化 Vital 属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffect;
	
private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
