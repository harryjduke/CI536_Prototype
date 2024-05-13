// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetPatrolPoint.generated.h"

/**
 * BTTask that sets the PatrolPathPointKey to the patrol path point from the EnemyAIController's patrol path at the
 * index stored in the PatrolPathIndexKey
 */
UCLASS()
class CI536_PROTOTYPE_API UBTTask_GetPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GetPatrolPoint();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	/**
	 * Int Input
	 * The key that stores the index of the next point in the patrol path
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector PatrolPathIndexKey;

	/**
	 * Vector Output
	 * The key that stores the index of the next point in the patrol path
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector PatrolPathPointKey;
};
