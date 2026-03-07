// Copyright ANNIXA.


#include "TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		/** Client */
		
		SendMouseCursorData();
	}
	else
	{
		/** Server */
		
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		// Returns TargetDataSet delegate for a given Ability/PredictionKey pair, then we Binding it.
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
		
		// 如果已发送了目标数据确认/取消事件，则调用这些事件
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		
		if (!bCalledDelegate)
		{
			// 当能力任务正在等待远程玩家数据时触发此事件。如果远程玩家提前终止了该能力，并且带有此设置的任务仍在运行，那么该能力将会被终止。
			// 换句话说，如果不调用 delegate，那么我们需要调用 SetWaitingOnRemotePlayerData，这将告诉服务器，我们仍需要在等待远程玩家数据。
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 在当前函数范围内所做的事情都应该被预测（这就是 FScopedPredictionWindow 的作用，Window 指的是这里的作用域）
	// 所以服务器，请允许我们在本地做我们正在做的事情，没有问题。
	// 然后服务器会在服务器知道它的时候执行它。这就是我们如何在能力任务函数中预测事物。
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// Ability：能力任务知道它们拥有的能力
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// 将数据复制到服务器上
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 这里我们正常广播
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// 告诉 ASC 目标数据我们已经收到了，不需要再缓存了，清除这些
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
