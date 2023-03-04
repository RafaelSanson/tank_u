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
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(TankShootAtPositionAction, ETriggerEvent::Started, this, &Atank_uPlayerController::OnTankShootAtPositionStarted);
	}
}

void Atank_uPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	SetupActionBindings();
}

void Atank_uPlayerController::OnTankShootAtPositionStarted()
{
	// We look for the location in the world where the player has pressed the input
	// If we hit a surface, cache the location
	if (FHitResult Hit; GetHitResultUnderCursor(ECC_Visibility, true, Hit))
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	if (Atank_uCharacter* ControlledCharacter = Cast<Atank_uCharacter>(GetPawn()); ControlledCharacter != nullptr)
	{
		const FVector WorldDirection = (CachedDestination - ControlledCharacter->GetActorLocation()).GetSafeNormal();
		ControlledCharacter->AddMovementInput(WorldDirection, 1.0, false);
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
}