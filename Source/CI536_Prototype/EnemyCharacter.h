// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
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

private:
	
	/** The behavior tree for this enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

};
