// Copyright Epic Games, Inc. All Rights Reserved.

#include "tank_uPlayerController.h"
#include "GameFramework/Pawn.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "tank_uCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

Atank_uPlayerController::Atank_uPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
}

void Atank_uPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void Atank_uPlayerController::SetupActionBindings()
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent == nullptr) return;

	// Setup mouse input events
	EnhancedInputComponent->BindAction(TankShootAtPositionAction, ETriggerEvent::Started, this, &Atank_uPlayerController::OnTankShootAtPositionStarted);

	// Setup keyboard input events
	EnhancedInputComponent->BindAction(TankMovementAction, ETriggerEvent::Triggered, this, &Atank_uPlayerController::OnTankMovement);
	EnhancedInputComponent->BindAction(TankTurnInPlaceAction, ETriggerEvent::Triggered, this, &Atank_uPlayerController::OnTankTurnInPlaceTriggered);
}

void Atank_uPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	SetupActionBindings();
}

void Atank_uPlayerController::OnTankMovement(const FInputActionValue& Value) 
{
	if (Atank_uCharacter* ControlledCharacter = Cast<Atank_uCharacter>(GetPawn()); ControlledCharacter != nullptr)
	{
		const FVector WorldDirection = ControlledCharacter->GetActorForwardVector();
		ControlledCharacter->AddMovementInput(WorldDirection, Value.Get<float>(), false);
	}
}

void Atank_uPlayerController::OnTankTurnInPlaceTriggered(const FInputActionValue& Value)
{
	if (Atank_uCharacter* ControlledCharacter = Cast<Atank_uCharacter>(GetPawn()); ControlledCharacter != nullptr)
	{
		const FVector WorldDirection = ControlledCharacter->GetActorForwardVector();
		ControlledCharacter->AddActorLocalRotation(FRotator(0, Value.Get<float>() * GetWorld()->DeltaTimeSeconds * 100.f, false));
	}
}

void Atank_uPlayerController::OnTankShootAtPositionStarted()
{
	FHitResult Hit;
	if ( !GetHitResultUnderCursor(ECC_Visibility, true, Hit)) return;
	
	CachedDestination = Hit.Location;
	
	// Move towards mouse pointer or touch
	if (Atank_uCharacter* ControlledCharacter = Cast<Atank_uCharacter>(GetPawn()); ControlledCharacter != nullptr)
	{
		const FVector WorldDirection = (CachedDestination - ControlledCharacter->GetActorLocation()).GetSafeNormal();
		ControlledCharacter->AddMovementInput(WorldDirection, 1.0, false);
	}

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
}

