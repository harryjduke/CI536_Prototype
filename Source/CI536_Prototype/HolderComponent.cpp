// Copyright (c) 2024 Harry Duke

#include "HolderComponent.h"

// Sets default values for this component's properties
UHolderComponent::UHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HeldHoldable = nullptr;
	if(const USceneComponent* ChildComponent = GetChildComponent(0))
	{
		if(AHoldable* ChildHoldable = Cast<AHoldable>(ChildComponent->GetAttachParentActor()))
		{
			HeldHoldable = ChildHoldable;
		}
	}
}

bool UHolderComponent::HoldHoldable(AHoldable* Holdable)
{
	if(!bFilterAllowedHoldables || HoldableAllowList.Contains(Holdable->GetClass()))
	{
		if(HeldHoldable != nullptr)
		{
			HeldHoldable->Drop();
		}
		if(Holdable->Hold(this))
		{
			HeldHoldable = Holdable;
			return true;
		}
	}
	return false;
}

void UHolderComponent::DropHeldHoldable()
{
	if(HeldHoldable != nullptr)
	{
		HeldHoldable->Drop();
        HeldHoldable = nullptr;
	}
}

bool UHolderComponent::TransferHeldHoldable(UHolderComponent* ReceivingHolderComponent,
	const bool bForceGiveTransfer, const bool bForceReceiveTransfer)
{
	if (ReceivingHolderComponent == nullptr)
	{
		return false;
	}
	if(!ReceivingHolderComponent->bFilterAllowedHoldables ||
		ReceivingHolderComponent->HoldableAllowList.Contains(HeldHoldable->GetClass()))
	{
		if ((bCanTransfer || bForceGiveTransfer) && (ReceivingHolderComponent->bCanTransfer || bForceReceiveTransfer) &&
			HeldHoldable != nullptr && ReceivingHolderComponent->GetHeldHoldable() == nullptr)
		{
			HeldHoldable->Transfer(ReceivingHolderComponent);
			ReceivingHolderComponent->HeldHoldable = HeldHoldable;
			HeldHoldable = nullptr;
			return true;
		}
	}
	return false;
}

