// Copyright ANNIXA.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	void AutoRun();
	
public:
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);
	
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	FORCEINLINE void ShiftPressed() { bShiftKeyDown = true; };
	FORCEINLINE void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;
	
	/** 角色移动输入处理 */
	void Move(const FInputActionValue& InputActionValue);
	
	/** 光标信息追踪处理，高亮显示相关 Actor */
	void CursorTrace();
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent = nullptr;

	UAuraAbilitySystemComponent* GetASC();
	
	/**
	 * 点击移动
	 */
	/** 长按会在每帧调用 AddMovementInput，此时 bAutoRunning == false */
	FVector CachedDestination = FVector::ZeroVector;
	// 跟随光标的时间（判断我们是否只是短暂点击）
	float FollowTime = 0.0f;
	float ShortPressThreshold = 0.5f;
	// 短按的时候会自动生成平滑的样条曲线，当我们越来越接近目的地时我们也应该取消自动寻路
	bool bAutoRunning = false;
	// 我们是否瞄准了某个目标，比如鼠标左键点中了敌人（检查我们的 ThisActor）
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.0f;

	// 样条曲线允许我们根据一个世界位置（FVector）生成一条平滑的曲线
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> ClickMoveSpline;
	
	FHitResult CursorResult;
	
	/** 伤害飘字组件 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
