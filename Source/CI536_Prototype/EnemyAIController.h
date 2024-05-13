// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * The AIController class for enemies, creates the blackboard, starts the behavior tree and configures the sight sense
 */
UCLASS()
class CI536_PROTOTYPE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController();

protected:

	virtual void OnPossess(APawn* InPawn) override;

private:
	
	/**
	 * Used to configure the sight sense
	 */
	UPROPERTY(EditDefaultsOnly, Category = Perception)
	class UAISenseConfig_Sight* SightConfig;
};
