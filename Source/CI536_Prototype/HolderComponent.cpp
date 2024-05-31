// Copyright (c) 2024 Harry Duke

#include "HolderComponent.h"

// Sets default values for this component's properties
UHolderComponent::UHolderComponent(): bFilterAllowedHoldables(false), bCanTransfer(false), HeldHoldable(nullptr)
{
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
			OnHeldHoldableChanged.Broadcast(HeldHoldable);
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
		OnHeldHoldableChanged.Broadcast(nullptr);
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
			ReceivingHolderComponent->OnHeldHoldableChanged.Broadcast(ReceivingHolderComponent->HeldHoldable);
			HeldHoldable = nullptr;
			OnHeldHoldableChanged.Broadcast(nullptr);
			return true;
		}
	}
	return false;
}

