// Copyright (c) 2024 Harry Duke

#include "BTService_UpdatePlayerLocation.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

UBTService_UpdatePlayerLocation::UBTService_UpdatePlayerLocation()
{
	//Ensure that the keys assigned to this service are of the correct type
	PlayerLocation.AddVectorFilter(this, PlayerLocation.SelectedKeyName);
}

void UBTService_UpdatePlayerLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset(); ensure(BBAsset))
	{
		PlayerLocation.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (const FVector PerceivedPlayerLocation = OwnerComp.GetAIOwner()->GetPerceptionComponent()->
			GetActorLocation(*PlayerCharacter); PerceivedPlayerLocation != FAISystem::InvalidLocation)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(PlayerLocation.SelectedKeyName, PerceivedPlayerLocation);
		}
	}
}
