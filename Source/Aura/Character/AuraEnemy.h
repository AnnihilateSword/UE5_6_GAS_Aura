// Copyright ANNIXA.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/Data/CharacterClassInfo.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "Aura/UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	virtual void BeginPlay() override;
	
	/** 初始化 Ability Actor Info */
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	
public:
	// ~Begin IEnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// ~End IEnemyInterface
	
	//~ Begin ICombatInterface
	virtual int32 GetPlayerLevel() override;	
	virtual void Die() override;
	//~ End ICombatInterface
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	/** 角色等级 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")	
	int32 Level = 1;

	/** 角色职业 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")	
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	/** 是否受击中 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	
	/** 基础移动速度 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.0f;
	
	/** 死亡后的生命周期 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.0f;
	
	/** AI Logic */
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
