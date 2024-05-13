// Copyright (c) 2024 Harry Duke


#include "BTTask_GetPatrolPoint.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetPatrolPoint::UBTTask_GetPatrolPoint()
{
	// Ensure that the assigned keys are of the correct type
	PatrolPathPointKey.AddVectorFilter(this, PatrolPathPointKey.SelectedKeyName);
	PatrolPathIndexKey.AddIntFilter(this, PatrolPathIndexKey.SelectedKeyName);
}

void UBTTask_GetPatrolPoint::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset(); ensure(BBAsset))
	{
		PatrolPathPointKey.ResolveSelectedKey(*BBAsset);
		PatrolPathIndexKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_GetPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			const int32 Index = BlackboardComponent->GetValueAsInt(PatrolPathIndexKey.SelectedKeyName);

			if (const AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyAIController->GetPawn()))
			{
				if (const APatrolPath* PatrolPath = EnemyCharacter->GetPatrolPath())
				{
					if (PatrolPath->GetNumPoints() >= Index + 1)
					{
						const FVector PatrolPoint = PatrolPath->GetPatrolPoint(Index);

						// Convert the local vector to global
						const FVector GlobalPatrolPoint =
							EnemyCharacter->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPoint);

						BlackboardComponent->SetValueAsVector(PatrolPathPointKey.SelectedKeyName, GlobalPatrolPoint);

						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

