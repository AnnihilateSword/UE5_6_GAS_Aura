// Copyright ANNIXA.


#include "AuraHUD.h"

#include "Aura/UI/Widget/AuraUserWidget.h"
#include "WidgetController/OverlayWidgetController.h"

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
	OverlayWidget->AddToViewport();
}

