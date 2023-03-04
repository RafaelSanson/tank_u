// Copyright Epic Games, Inc. All Rights Reserved.

#include "tank_uGameMode.h"
#include "tank_uPlayerController.h"
#include "tank_uCharacter.h"
#include "UObject/ConstructorHelpers.h"

Atank_uGameMode::Atank_uGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = Atank_uPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}