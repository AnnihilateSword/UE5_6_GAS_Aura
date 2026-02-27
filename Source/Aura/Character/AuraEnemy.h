// Copyright ANNIXA.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;
	
public:
	// ~Begin IEnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// ~End IEnemyInterface
};
