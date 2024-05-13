// Copyright (c) 2024 Harry Duke


#include "PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

}

FVector APatrolPath::GetPatrolPoint(int const Index) const
{
	if (PatrolPoints.Num() >= Index)
	{
		return PatrolPoints[Index];
	}
	return FVector::Zero();
}

int APatrolPath::GetNumPoints() const
{
	return PatrolPoints.Num();
}

