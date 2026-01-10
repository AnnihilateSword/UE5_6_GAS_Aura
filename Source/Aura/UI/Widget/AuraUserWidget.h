// Copyright ANNIXA.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:
	/** 每当我们为给定的 Widget 设置 WidgetController 时，我们也会调用此函数 */
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

public:
	// 这里使用的时候转换成具体的 WidgetController 类
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
};
