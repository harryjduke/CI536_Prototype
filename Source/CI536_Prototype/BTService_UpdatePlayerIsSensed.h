// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdatePlayerIsSensed.generated.h"

/**
 * BTService that, while active, checks if the player is currently being sensed
 */
UCLASS()
class CI536_PROTOTYPE_API UBTService_UpdatePlayerIsSensed : public UBTService
{
	GENERATED_BODY()

	UBTService_UpdatePlayerIsSensed();

protected:
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/**
	 * Bound to the OnPerceptionUpdated delegate of the AIController
	 * Finds the player from the updated actors (if present) and updates the blackboard
	 * @param UpdatedActors The actors that have had their perception info updated
	 */
	UFUNCTION()
	void HandlePerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	/**
	 * Updates the PlayerSensed and PlayerLocation blackboard keys based on the perception info that the AIController
	 * holds on the player
	 * @param PlayerCharacter The player that will be used to update the blackboard
	 */
	void UpdateBlackboard(const APlayerCharacter* PlayerCharacter) const;

private:

	/**
	 * Bool
	 * The key that stores whether the player is currently being sensed
	 */ 
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector PlayerSensed;

	/**
	 * A cached value of behaviour tree component that this service is used on
	 * Updated whenever this node becomes relevant
	 */
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerBehaviorTreeComponent;
};
