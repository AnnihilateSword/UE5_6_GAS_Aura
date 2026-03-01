// Copyright ANNIXA.


#include "AuraHUD.h"

#include "Aura/UI/Widget/AuraUserWidget.h"
#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"
#include "Aura/UI/WidgetController/OverlayWidgetController.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class 未初始化，请在 BP_AuraHUD 初始化"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class 未初始化，请在 BP_AuraHUD 初始化"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	// 广播玩家初始属性值
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

