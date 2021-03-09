// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HackNSlashProtoPlayerController.generated.h"

UCLASS()
class AHackNSlashProtoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHackNSlashProtoPlayerController();

	void RotateTowardCursor();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	template <uint32 Index>
	void ExecuteAction()
	{
		ExecuteAction(Index);
	}
	
	void ExecuteAction(uint32 index) const;

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);


	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


