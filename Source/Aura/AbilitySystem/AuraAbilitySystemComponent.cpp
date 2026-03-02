// Copyright ANNIXA.


#include "AuraAbilitySystemComponent.h"

#include "Abilities/AuraGameplayAbility.h"
#include "Aura/AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	/** 绑定相关委托 */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			// 为启动能力添加启动标签
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupGameplayTag);
			GiveAbility(AbilitySpec);
			
			// 你也可以选择在授予能力之后并立即激活一次能力
			// GiveAbilityAndActivateOnce(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// AbilitySpec Input Pressed
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// AbilitySpec Input Released
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
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
