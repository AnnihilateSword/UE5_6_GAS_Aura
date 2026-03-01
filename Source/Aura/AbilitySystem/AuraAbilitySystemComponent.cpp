// Copyright ANNIXA.


#include "AuraAbilitySystemComponent.h"

#include "Aura/AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	/** 绑定相关委托 */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied);
}

void UAuraAbilitySystemComponent::RPCClient_EffectAssetTagBroadcast_Implementation(const FGameplayTagContainer& Container)
{
	EffectAssetTag.Broadcast(Container);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	// EffectSpec.GetAllGrantedTags(TagContainer);
	
	// for (const FGameplayTag& Tag : TagContainer)
	// {
	// 	//TODO: Broadcast the tag to the Widget Controller
	// 	const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	// }
	
	// [Server]
	EffectAssetTag.Broadcast(TagContainer);
	// [Client]
	RPCClient_EffectAssetTagBroadcast(TagContainer);
}
