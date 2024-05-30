// Copyright (c) 2024 Harry Duke

#include "Holdable.h"

#include "HolderComponent.h"

// Sets default values
AHoldable::AHoldable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a UStaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	
	bIsHeld = false;
}

bool AHoldable::Hold(UHolderComponent* HolderComponent)
{
	if(!bIsHeld)
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent()))
		{
			PrimitiveComponent->SetSimulatePhysics(false);
		}
	
		AttachToComponent(Cast<USceneComponent>(HolderComponent), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		bIsHeld = true;
		return  true;
	}
	return false;
}

void AHoldable::Drop()
{
	if(bIsHeld)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent()))
		{
			PrimitiveComponent->SetSimulatePhysics(true);
		}

		bIsHeld = false;
	}
}

void AHoldable::Transfer(UHolderComponent* HolderComponent)
{
	AttachToComponent(Cast<USceneComponent>(HolderComponent), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void AHoldable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoldable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

