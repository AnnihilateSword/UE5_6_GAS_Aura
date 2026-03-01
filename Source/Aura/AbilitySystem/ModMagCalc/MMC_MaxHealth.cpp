// Copyright ANNIXA.


#include "MMC_MaxHealth.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	/**
	 * 告诉 GAS：这个 MMC 在计算数值时，需要读取目标身上的 Vigor 属性。
	 */
	
	// 指定我们要捕获的属性：Vigor（活力）
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

	// 指定这个属性来自哪里
	// Target = 目标身上
	// Source = 施法者身上
	// 这里计算的是角色自身的最大生命值，所以从 Target 读取
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// 是否使用快照
	// true  = 在 GE 应用瞬间抓一次值，以后不再更新
	// false = 每次重新计算时都动态读取当前值
	// MaxHealth 需要随 Vigor 变化而变化，所以不能快照
	VigorDef.bSnapshot = false;

	// 把这个属性捕获定义注册到 RelevantAttributesToCapture
	// 如果不注册，GAS 不会帮我们捕获这个属性
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	/*************************/
	/** 在这里返回我们的计算值 */
	/*************************/

	// Gather tags from source and target.（这个我们这里暂时没用，只是示范下获取方法）
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Vigor = 0.0f;
	// 获取我们这个初始化设置的 Vigor Magnitude!!
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParameters, Vigor);
	// 确保 Vigor 不会是负值
	Vigor = FMath::Max(Vigor, 0.0f);

	// 我们希望 MaxHealth 不仅取决于活力 Vigor，还取决于角色的等级 Level
	// 只要这个 GE 有一个 Source Object，我们就可以把它 Cast
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	return 80.0f + 2.5f * Vigor + 10.0f * PlayerLevel;
}
