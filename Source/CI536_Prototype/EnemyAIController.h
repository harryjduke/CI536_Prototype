// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CI536_PROTOTYPE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AEnemyAIController(FObjectInitializer const& ObjectInitializer);

protected:

	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Perception)
	class UAISenseConfig_Sight* SightConfig;
};
