// Copyright (c) 2024 Harry Duke


#include "BTService_UpdatePlayerIsSensed.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

UBTService_UpdatePlayerIsSensed::UBTService_UpdatePlayerIsSensed()
{
	CachedOwnerBehaviorTreeComponent = nullptr;

	//Ensure that the keys assigned to this service are of the correct type
	PlayerSensed.AddBoolFilter(this, PlayerSensed.SelectedKeyName);

	//This means OnBecomeRelevant and OnCeaseRelevant will be called
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	
}

void UBTService_UpdatePlayerIsSensed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//Cache the owning Behavior Tree Component so it can be used to update the blackboard keys in bound methods
	CachedOwnerBehaviorTreeComponent = &OwnerComp;

	//Start handling perception updates
	OwnerComp.GetAIOwner()->GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this,
		&UBTService_UpdatePlayerIsSensed::HandlePerceptionUpdate);

	//Set the initial values of the blackboard keys
	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		UpdateBlackboard(PlayerCharacter);
	}
}

void UBTService_UpdatePlayerIsSensed::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	//Stop handling perception updates
	OwnerComp.GetAIOwner()->GetPerceptionComponent()->OnPerceptionUpdated.RemoveDynamic(this,
		&UBTService_UpdatePlayerIsSensed::HandlePerceptionUpdate);
}

// ReSharper disable once CppMemberFunctionMayBeConst because const functions cannot be bound to delegates
void UBTService_UpdatePlayerIsSensed::HandlePerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	APlayerCharacter** PlayerCharacter = nullptr;
	UpdatedActors.FindItemByClass<APlayerCharacter>(PlayerCharacter);
	if (PlayerCharacter)
	{
		UpdateBlackboard(*PlayerCharacter);
	}
}

void UBTService_UpdatePlayerIsSensed::UpdateBlackboard(const APlayerCharacter* PlayerCharacter) const
{
	if (CachedOwnerBehaviorTreeComponent)
	{
		if(const FActorPerceptionInfo* PlayerPerceptionInfo =
			CachedOwnerBehaviorTreeComponent->GetAIOwner()->GetPerceptionComponent()->GetActorInfo(*PlayerCharacter))
		{
			//Update PlayerSensed
			CachedOwnerBehaviorTreeComponent->GetBlackboardComponent()->SetValueAsBool(
				PlayerSensed.SelectedKeyName,
				PlayerPerceptionInfo->HasAnyCurrentStimulus()
			);
		}
	}
}


