// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNextPatrolPathIndex.generated.h"

/**
 * A BTTask that gets the index of the next point in the patrol path, will loop to the start of the patrol path once
 * completed unless bIsBidirectional is set
 */
UCLASS()
class CI536_PROTOTYPE_API UBTTask_GetNextPatrolPathIndex : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetNextPatrolPathIndex();
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	/**
	 * Int Input Output
	 * The key that stores the index of the next point in the patrol path
     */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector PatrolPathIndexKey;
	
	/** An enum that stores the direction that the AI will follow the patrol path in */
	enum class EDirectionType {Forward, Reverse};

	/** The direction the AI is currently following the patrol path in */
	EDirectionType Direction = EDirectionType::Forward;

	/** If true the AI will follow the patrol path in reverse once it is completed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bBiDirectional = false;
};
