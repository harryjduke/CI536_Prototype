// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdatePlayerLocation.generated.h"

/**
 * BTService that, while active, updates the given key to be the sensed player location
 * @deprecated use EQS instead
 */
UCLASS()
class CI536_PROTOTYPE_API UBTService_UpdatePlayerLocation : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdatePlayerLocation();
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
	/**
	 * Vector Output
	 * The key that stores the last sensed location of the player
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector PlayerLocation;
};
