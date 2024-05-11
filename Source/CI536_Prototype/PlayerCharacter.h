// Copyright (c) 2024 Harry Duke

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

/**
 * APlayerCharacter
 *
 * The character class for the player which extends ACharacter and implements jumping, crouching, sprinting, health,
 * healing, taking damage and dying.
 */
UCLASS(config=Game)
class CI536_PROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

	virtual void Tick(float DeltaSeconds) override;

	// COMPONENTS
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// MOVEMENT
	
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
	 * Offset the camera view by CrouchEyeOffset to control the crouching transition
	 * @see AActor::CalcCamera
	 * @param	DeltaTime	Delta time seconds since last update
	 * @param	OutResult	Camera configuration
	 */
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;

	// HEALTH
	
	/**
	 * Deal damage to the character and kill them if health 0
	 * @see https://www.unrealengine.com/blog/damage-in-ue4
	 * @param DamageAmount		How much damage to apply
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 * @return					The amount of damage actually applied.
	 */
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
							 AActor* DamageCauser) override;

	
	/** Destroy character and tell the game mode to restart the player */
	UFUNCTION(BlueprintCallable)
	void Die();
	
protected:
	virtual void BeginPlay() override;

	// INPUT
	
	/** Called on tick to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// MOVEMENT

	/** How fast the view will move when crouching in m/s */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float CrouchSpeed;

	/** The speed at which the character will be walking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float WalkSpeed;

	/** The speed at which the character will be sprinting*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float SprintSpeed;

	/** How much the view is currently offset from the eye position */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FVector CrouchEyeOffset;
	
	/**
	 * Bound to movement input
	 * Apply movement to the player character based on input
	 */
	void Move(const FInputActionValue& Value);

	/**
	 * Bound to look input
	 * Apply rotation to the controller to move a boom arm with rotation set to use controller rotation
	 */
	void Look(const FInputActionValue& Value);

	/**
	 * Bound to crouch input
	 * Called to check if the character is grounded before crouching
	 */
	void StartCrouch();

	/**
	 * Called on tick to Wind CrouchEyeOffset down to 0, smoothly removing the view offset
	 *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 *	@return The value of CrouchEyeOffset after this frame
	 */
	FVector WindDownCrouchEyeOffset(float DeltaSeconds);

	/**
	 * Called for starting sprinting input
	 * Set the MaxWalkSpeed of the movement component to the SprintSpeed
	 */
	void StartSprinting ();


	/**
	 * Called for stopping sprinting input
	 * Set the MaxWalkSpeed of the movement component to the WalkSpeed
	 */
	void StopSprinting();

	// Health

	/**
	* Called on tick to check if the character is ready to heal and if so apply the healing for that frame
	*	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	*	@return The value of health after this frame
	*/
	float HealIfReady(float DeltaSeconds);

	/** The maximum health of the player character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth;
	
	/** The time in seconds from when the player character takes damage till when they start healing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float HealCooldown;

	/** The rate at which the player character heals in health points per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float HealRate;
	
	/** The current health of the player character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Health)
	float Health;

	/** The world time in seconds since the player character last took damage */
	UPROPERTY(BlueprintReadWrite, Category = Health)
	float LastDamagedTime;
	
private:
	// Components
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** 

	// Input
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
};