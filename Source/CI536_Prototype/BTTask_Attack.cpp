// Copyright (c) 2024 Harry Duke


#include "BTTask_Attack.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DamageEvents.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = FString("Attack Player");
	
	// Filter blackboard key to only accept APlayerCharacters
	BlackboardKey.AddObjectFilter(this, BlackboardKey.SelectedKeyName, APlayerCharacter::StaticClass());
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(BlackboardComponent->GetValueAsObject((BlackboardKey.SelectedKeyName))))
		{
			if (AAIController* AIController = OwnerComp.GetAIOwner())
			{
				PlayerCharacter->TakeDamage(Damage, FDamageEvent(), AIController, AIController->GetCharacter());
			}
		}
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
