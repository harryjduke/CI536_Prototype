// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"

/**
 * BTTask that applies damage to the APlayerCharacter in the blackboard key
 */
UCLASS()
class CI536_PROTOTYPE_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Attack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	/** The amount of damage to deal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float Damage = 10;
};
