// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChangeMoveSpeed.generated.h"

/**
 * A BTService that, when activated, changes the movement speed of the controlled character by a multiplier and, when
 * deactivated, changes it back
 */
UCLASS()
class CI536_PROTOTYPE_API UBTService_ChangeMoveSpeed : public UBTService
{
	GENERATED_BODY()
	
public:
	
	UBTService_ChangeMoveSpeed();
	
protected:
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float SpeedMultiplier = 1;
	
};
