// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "vr02_lag_cpp.h"
#include "vr02_lag_cppCharacter.h"

//////////////////////////////////////////////////////////////////////////
// Avr02_lag_cppCharacter

Avr02_lag_cppCharacter::Avr02_lag_cppCharacter() : Delay(0), m_gameTime(0)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void Avr02_lag_cppCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &Avr02_lag_cppCharacter::doJump);
	InputComponent->BindAction("Jump", IE_Released, this, &Avr02_lag_cppCharacter::doStopJump);

	InputComponent->BindAxis("MoveForward", this, &Avr02_lag_cppCharacter::doMoveForward);
	InputComponent->BindAxis("MoveRight", this, &Avr02_lag_cppCharacter::doMoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &Avr02_lag_cppCharacter::doAddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &Avr02_lag_cppCharacter::doTurnAtRate);
	InputComponent->BindAxis("LookUp", this, &Avr02_lag_cppCharacter::doAddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &Avr02_lag_cppCharacter::doLookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &Avr02_lag_cppCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &Avr02_lag_cppCharacter::TouchStopped);
}

void Avr02_lag_cppCharacter::doAddControllerYawInput(float Val) {
	InputEvent e(m_gameTime);
	e.m_action.BindUObject(this, &APawn::AddControllerYawInput, Val);
	m_inputQueue.Enqueue(e);
}

void Avr02_lag_cppCharacter::doAddControllerPitchInput(float Val) {
	InputEvent e(m_gameTime);
	e.m_action.BindUObject(this, &APawn::AddControllerPitchInput, Val);
	m_inputQueue.Enqueue(e);
}


void Avr02_lag_cppCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		doJump();
	}
}

void Avr02_lag_cppCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		doStopJump();
	}
}

void Avr02_lag_cppCharacter::doJump() {
	InputEvent e(m_gameTime);
	e.m_action.BindUObject(this, &Avr02_lag_cppCharacter::Jump);
	m_inputQueue.Enqueue(e);
}

void Avr02_lag_cppCharacter::doStopJump() {
	InputEvent e(m_gameTime);
	e.m_action.BindUObject(this, &Avr02_lag_cppCharacter::StopJumping);
	m_inputQueue.Enqueue(e);
}

void Avr02_lag_cppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Avr02_lag_cppCharacter::doTurnAtRate(float Rate)
{
	InputEvent e(m_gameTime);
	e.m_action.BindUObject(this, &Avr02_lag_cppCharacter::AddControllerYawInput, Rate);
	m_inputQueue.Enqueue(e);
}

void Avr02_lag_cppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Avr02_lag_cppCharacter::doLookUpAtRate(float Rate)
{
	InputEvent e(m_gameTime);
	e.m_action.BindUObject(this, &Avr02_lag_cppCharacter::LookUpAtRate, Rate);
	m_inputQueue.Enqueue(e);
}

void Avr02_lag_cppCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Avr02_lag_cppCharacter::doMoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		InputEvent e(m_gameTime);
		e.m_action.BindUObject(this, &Avr02_lag_cppCharacter::MoveForward, Value);
		m_inputQueue.Enqueue(e);
	}
}

void Avr02_lag_cppCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void Avr02_lag_cppCharacter::doMoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		InputEvent e(m_gameTime);
		e.m_action.BindUObject(this, &Avr02_lag_cppCharacter::MoveRight, Value);
		m_inputQueue.Enqueue(e);
	}
}

void Avr02_lag_cppCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Call parent class tick function  

	m_gameTime += DeltaTime;

	InputEvent e;
	while (m_inputQueue.Peek(e) && m_gameTime - e.m_start > Delay )
	{
		m_inputQueue.Dequeue(e);
		e.m_action.ExecuteIfBound();
	}
}