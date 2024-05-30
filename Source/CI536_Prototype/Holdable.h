// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Holdable.generated.h"

class UHolderComponent;

UCLASS()
class CI536_PROTOTYPE_API AHoldable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoldable();

	/** True if this actor is currently held by a UHolderComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsHeld;

	/**
	 * Disables physics and attaches this actor to the given UHolderComponent.
	 * @param HolderComponent The UHolderComponent to attach this actor to.
	 * @return true if this actor was successfully held by the UHolderComponent, false if not.
	 */
	bool Hold(UHolderComponent* HolderComponent);

	/**
	 * Detaches this actor from it's parent actor and re-enables physics
	 */
	void Drop();

	/**
	 * Attaches this actor to the given UHolderComponent
	 * @param HolderComponent The UHolderComponent to transfer this actor to
	 */
	void Transfer(UHolderComponent* HolderComponent);

private:
	/** The root static mesh component that handles physics. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Holdable, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
};
