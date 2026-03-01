// Copyright ANNIXA.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	/** 如果有 OverlayWidgetController 则返回 OverlayWidgetController，如果没有就重新创建一个并存在 OverlayWidgetController */
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	/** 
	 * 初始化 OverlayWidget
	 * 没有放在 BeginPlay 中是因为，在 BeginPlay 可能有些值未初始化
	 * 决定放在哪里调用 InitOverlay：我们什么时候知道它们都已经用有效数据初始化了？
	 * 
	 * 一个很好的地方是在 AAuraCharacter::InitAbilityActorInfo()，所以我们会在这里调用该函数
	 */
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	
private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
