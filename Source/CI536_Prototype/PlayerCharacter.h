// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

UCLASS(config=Game)
class CI536_PROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** How much the view is offset from the eye position */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Crouch, meta = (AllowPrivateAccess = "true"))
	FVector CrouchEyeOffset;

	/** How quickly CrouchEyeOffset will return to zero */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Crouch, meta = (AllowPrivateAccess = "true"))
	float CrouchSpeed;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for starting sprinting input */
	void StartSprinting ();

	/** Called for stopping sprinting input*/
	void StopSprinting();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called to check if the character is grounded before crouching*/
	void StartCrouch();

	// To add mapping context
	virtual void BeginPlay() override;

	// To wind CrouchEyeOffset Down to zero
	virtual void Tick(float DeltaTime) override;

public:	
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/**
	 * Implementation of OnStartCrouch for PlayerCharacter.
	 * Set CrouchEyeOffset to initially negate the crouch camera offset effect (through CalcCamera).
	 */
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/**
	 * Implementation of OnEndCrouch for PlayerCharacter.
	 * Set CrouchEyeOffset to initially negate the crouch camera offset effect (through CalcCamera).
	 */
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/**
	 * Implementation of CalcCamera for PlayerCharacter
	 * Offset the camera view by CrouchEyeOffset
	 */
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;

};
