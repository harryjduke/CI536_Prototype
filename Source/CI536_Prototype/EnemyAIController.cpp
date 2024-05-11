// Copyright (c) 2024 Harry Duke

#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
	// Setup perception system
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(FName("Perception Component")));
		SightConfig->SightRadius = 3000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(.3f) ;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AAIController::GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		AAIController::GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Create the blackboard component and store it
	if (const AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn))
	{
		if (UBehaviorTree* const BehaviorTree = EnemyCharacter->GetBehaviourTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(BehaviorTree);
		}
	}
}