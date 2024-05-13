// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "PatrolPath.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class CI536_PROTOTYPE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	/** Returns the behaviour tree for this character */
	FORCEINLINE UBehaviorTree* GetBehaviourTree() const { return BehaviorTree; }

	/** Returns the patrol path for this character */
	FORCEINLINE APatrolPath* GetPatrolPath() const { return PatrolPath; }

private:
	
	/** The behavior tree for this enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;
};
