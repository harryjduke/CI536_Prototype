#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);

//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don 't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	WalkSpeed = 500.f;
	SprintSpeed = 1000.f;
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Configure crouching
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 12.f;

	// Configure Health
	MaxHealth = 100;
	Health = MaxHealth;
	HealCooldown = 7;
	HealRate = 10;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	WindDownCrouchEyeOffset(DeltaSeconds);

	HealIfReady(DeltaSeconds);
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACharacter::UnCrouch, false);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);
		
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//////////////////////////////////////////////////////////////////////////
// Movement

//Moving

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// Looking

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Crouching

void APlayerCharacter::StartCrouch()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		Crouch();
	}
}

FVector APlayerCharacter::WindDownCrouchEyeOffset(float DeltaSeconds)
{
	const float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaSeconds);
	return CrouchEyeOffset = (1.f - CrouchInterpTime) * CrouchEyeOffset;
}

void APlayerCharacter::OnStartCrouch(const float HalfHeightAdjust, const float ScaledHalfHeightAdjust)
{
	if(HalfHeightAdjust == 0.f) // Filter if there is no change in the height from the crouch
	{
		return;
	}

	const float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	/* Calculate the difference in eye height (above the collision center) and add the change in height of the
	 collision center to calculate how much the camera view would have to be offset by to negate change in camera
	 position caused by the crouch */
	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight + HalfHeightAdjust;
	if (FollowCamera)
	{
		FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);
	}
}

void APlayerCharacter::OnEndCrouch(const float HalfHeightAdjust, const float ScaledHalfHeightAdjust)
{
	if(HalfHeightAdjust == 0.f) // Check the character actually crouches any distance
	{
		return;
	}

	const float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	/* Calculate the difference in eye height (above the collision center) and add the change in height of the
	 collision center to calculate how much the camera view would have to be offset by to negate change in camera
	 position caused by the uncrouch */
	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight - HalfHeightAdjust;
	if (FollowCamera)
	{
		FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);
	}
}

// Sprinting

void APlayerCharacter::CalcCamera(const float DeltaTime, FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);
	if (FollowCamera)
	{
		FollowCamera->GetCameraView(DeltaTime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst because const functions are not compatible with BindAction
void APlayerCharacter::StartSprinting()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst because const functions are not compatible with BindAction
void APlayerCharacter::StopSprinting()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

//////////////////////////////////////////////////////////////////////////
// Health

float APlayerCharacter::TakeDamage(const float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{
	LastDamagedTime = GetWorld()->GetTimeSeconds();
	
	if (Health - DamageAmount <= 0)
	{
		Health = 0;
		Die();
		return Health;
	}

	return Health -= DamageAmount;
}

void APlayerCharacter::Die()
{
	this->Destroy();
}


float APlayerCharacter::HealIfReady(const float DeltaSeconds)
{
	if (GetWorld()->GetTimeSeconds() - LastDamagedTime > HealCooldown)
	{
		return Health = std::min(Health + HealRate * DeltaSeconds, MaxHealth);
	}
	return 0;
}