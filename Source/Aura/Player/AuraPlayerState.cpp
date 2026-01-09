// Copyright ANNIXA.


#include "AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	// 设置更快的网络更新频率，因为挂载了 AbilitySystemComponent 和 AttributeSet
	// 这意味着每秒更新 100 次
	SetNetUpdateFrequency(100.0f);
}
