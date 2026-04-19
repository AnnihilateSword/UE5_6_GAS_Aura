// Copyright ANNIXA.


#include "AuraEnemy.h"

#include "Aura/Aura.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/UI/Widget/AuraUserWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// 设置复制模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 初始化移动速度
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	check(AbilitySystemComponent);
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		// 初始化所有职业共享的 GA
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}

	// 设置 WidgetController
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	// 注册属性变化委托
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		// 监听 Effects_HitReact 标签添加或移除时触发的回调
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &ThisClass::HitReactTagChanged
		);
		
		// 在客户端，广播会在属性被复制时主动触发
		if (HasAuthority())
		{
			OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());	
			OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());	
		}
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	// Init Ability Actor Info
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->AbilityActorInfoSet();
	
	if (HasAuthority())
	{
		// 初始化属性
		InitializeDefaultAttributes();
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	// 根据职业，等级初始化默认属性值
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	/**
	 * 我们可以检查计数是否为 0，是否大于 0，是否第一次添加等等
	 */
	bHitReacting = NewCount > 0;
	// 如果在受击，不能移动
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : BaseWalkSpeed;
}
