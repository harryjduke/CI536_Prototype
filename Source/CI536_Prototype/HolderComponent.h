// Copyright (c) 2024 Harry Duke

#pragma once

#include "CoreMinimal.h"
#include "Holdable.h"
#include "Components/SceneComponent.h"
#include "HolderComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeldHoldableChanged, AHoldable*, NewHoldable);

/** A component that can hold and drop an AHoldable actor as well as transferring it between UHolderComponents */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CI536_PROTOTYPE_API UHolderComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHolderComponent();
	
	/** @return A pointer to the currently held AHoldable actor */
	FORCEINLINE AHoldable* GetHeldHoldable() const {return HeldHoldable;}

	/**
	 * If enabled, only subclasses of AHoldable specified in HoldableAllowList will be able to be held by and
	 * transferred to this component
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Holder, meta = (AllowPrivateAccess = "true"))
	bool bFilterAllowedHoldables;

	/**
	  * An array of AHoldable subclasses that are allowed to be held by and transferred to this component. Will only
	  * apply if bFilterAllowedHoldables is enabled
	  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Holder, meta = (AllowPrivateAccess = "true", EditCondition = "bFilterAllowedHoldables == true"))
	TArray<TSubclassOf<AHoldable>> HoldableAllowList;

	/**
	  * If enabled, AHoldable actors can be transferred to and from this component to another without the need for
	  * bForceGiveTransfer or bForceReceiveTransfer to be set to true when calling TransferHeldHoldable
	  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Holder, meta = (AllowPrivateAccess = "true"))
	bool bCanTransfer;

	/**
	 * Calls hold on the given Holdable object, passing this component as the holder, sets the HeldHoldable. Will drop
	 * any currently held actors.
	 * @param Holdable  A pointer to the AHoldable actor to be held by this component.
	 * @return True if the actor was successfully held, False if not.
	 */
	UFUNCTION(BlueprintCallable)
	bool HoldHoldable(AHoldable* Holdable);

	/**
	 * Drops the currently held AHoldable actor and sets HeldHoldable to nullptr.
	 */
	UFUNCTION(BlueprintCallable)
	void DropHeldHoldable();

	/**
	 * Transfers the currently held AHoldable actor to the given UHolderComponent. Sets the HeldHoldable of this
	 * component to nullptr and the HeldHoldable of the receiving component to the AHoldable actor.
	 * @param ReceivingHolderComponent  The component that will receive the AHoldable actor transferred from this
	 * component
	 * @param bForceGiveTransfer  If true, this component will transfer even if bCanTransfer is false. Intended to be
     * used when the caller is associated with this component and so always intents to transfer the AHoldable actor.
     * @param bForceReceiveTransfer  If true, the receiving component will receive the AHoldable actor even if
     * bCanTransfer is false. Intended to be used when the caller is associated with the receiving component and so
	 * always intents to receive the AHoldable actor.
	 * @return true if the AHoldable actor was successfully transferred, false if not
	 */
	UFUNCTION(BlueprintCallable)
	bool TransferHeldHoldable(UHolderComponent* ReceivingHolderComponent, bool bForceGiveTransfer = false, bool bForceReceiveTransfer = false);

	/**
	 * A delegate called whe the held holdable changes, used so inheriting blueprints can perform actions when a
	 * holdable is held.
	 */
	UPROPERTY(BlueprintAssignable, Category = Holder)
	FHeldHoldableChanged OnHeldHoldableChanged;
	
private:

	/** A pointer to the currently held AHoldable actor. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Holder, meta = (AllowPrivateAccess = "true"))
	AHoldable* HeldHoldable;
};
