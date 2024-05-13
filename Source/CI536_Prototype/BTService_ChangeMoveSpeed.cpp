// Copyright (c) 2024 Harry Duke


#include "BTService_ChangeMoveSpeed.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeMoveSpeed::UBTService_ChangeMoveSpeed()
{
	//This means OnBecomeRelevant and OnCeaseRelevant will be called
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

// Change the movement speed
void UBTService_ChangeMoveSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (const AController* AIController = OwnerComp.GetAIOwner())
	{
		if (const ACharacter* Character = AIController->GetCharacter())
		{
			if (UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement())
			{
				CharacterMovement->MaxWalkSpeed *= SpeedMultiplier;
			}
		}
	}
}

// Reset the movement speed
void UBTService_ChangeMoveSpeed::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	
	if (const AController* AIController = OwnerComp.GetAIOwner())
	{
		if (const ACharacter* Character = AIController->GetCharacter())
		{
			if (UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement())
			{
				CharacterMovement->MaxWalkSpeed /= SpeedMultiplier;
			}
		}
	}
}
