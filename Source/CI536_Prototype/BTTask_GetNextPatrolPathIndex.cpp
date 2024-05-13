// Copyright (c) 2024 Harry Duke


#include "BTTask_GetNextPatrolPathIndex.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetNextPatrolPathIndex::UBTTask_GetNextPatrolPathIndex()
{
	// Ensure that the assigned key is of the correct type
	PatrolPathIndexKey.AddIntFilter(this, PatrolPathIndexKey.SelectedKeyName);
}
	
void UBTTask_GetNextPatrolPathIndex::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset(); ensure(BBAsset))
	{
		PatrolPathIndexKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_GetNextPatrolPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (const AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn()))
		{
			if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
			{
				const int NumPoints = EnemyCharacter->GetPatrolPath()->GetNumPoints();
				const int MaxIndex = NumPoints - 1;
				int Index = BlackboardComponent->GetValueAsInt(PatrolPathIndexKey.SelectedKeyName);

				// Change direction if we are at the first or last index and we are in bidirectional mode
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == 0 && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// Write new value to the blackboard key. Increment if the direction is forward, else decrement.
				// Modulus keeps values within range
				BlackboardComponent->SetValueAsInt(PatrolPathIndexKey.SelectedKeyName,
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NumPoints);
				
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
