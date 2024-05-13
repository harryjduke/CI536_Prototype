// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

/**
 * An actor that holds a set of patrol points for an AI to follow
 */
UCLASS()
class CI536_PROTOTYPE_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:
	APatrolPath();
	
	/**
	 * Gets the position of the patrol point given the index, will return a zero vector if index is out of range so it
	 * is best to check the length of the path before calling @see APatrolPath::GetNumPoints
	 * @param Index The index of the patrol point to return
	 * @return The position of the patrol point at the given index
	 */
	FVector GetPatrolPoint(int const Index) const;

	
	/** @return The number of points in the patrol path */
	int GetNumPoints() const;
	
private:

	/** An array of points that make up the patrol path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = "true", AllowPrivateAccess))
	TArray<FVector> PatrolPoints;
};
