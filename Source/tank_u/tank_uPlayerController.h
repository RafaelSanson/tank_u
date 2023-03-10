// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "tank_uPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class Atank_uPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	Atank_uPlayerController();

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	#pragma region INPUT_ACTIONS
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* TankShootAtPositionAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* TankMovementAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* TankTurnInPlaceAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* QuitGame;

	#pragma endregion 

protected:
	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay() override;
	void OnTankMovement(const FInputActionValue& Value);
	void OnTankTurnInPlaceTriggered(const FInputActionValue& Value);
	void OnQuitGame();
	void SetupActionBindings();

	void OnTankShootAtPositionStarted();

private:
	FVector CachedDestination;
};


