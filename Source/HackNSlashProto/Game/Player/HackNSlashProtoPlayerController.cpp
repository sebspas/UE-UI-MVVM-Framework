// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HackNSlashProtoPlayerController.h"

#include "HackNSlashProtoCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HackNSlashProto/Game/Component/Health.h"
#include "HackNSlashProto/Game/Component/Mana.h"
#include "Kismet/KismetMathLibrary.h"

AHackNSlashProtoPlayerController::AHackNSlashProtoPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AHackNSlashProtoPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		if (AHackNSlashProtoCharacter* MyPawn = Cast<AHackNSlashProtoCharacter>(GetPawn()))
		{
			if(UActorAction* action = MyPawn->GetCurrentAction())
			{
				if(!action->CanMove)
				{
					MyPawn->CancelCurrentAction();
				}
			}
		}
		MoveToMouseCursor();
	}
}

void AHackNSlashProtoPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AHackNSlashProtoPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AHackNSlashProtoPlayerController::OnSetDestinationReleased);
	
	InputComponent->BindAction("Action1", IE_Released, this, &AHackNSlashProtoPlayerController::ExecuteAction<0>);
	InputComponent->BindAction("Action2", IE_Released, this, &AHackNSlashProtoPlayerController::ExecuteAction<1>);
	InputComponent->BindAction("Action3", IE_Released, this, &AHackNSlashProtoPlayerController::ExecuteAction<2>);
}

void AHackNSlashProtoPlayerController::ExecuteAction(uint32 index) const
{
	if (AHackNSlashProtoCharacter* MyPawn = Cast<AHackNSlashProtoCharacter>(GetPawn()))
	{
		MyPawn->ExecuteAction(index);
	}
}

void AHackNSlashProtoPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AHackNSlashProtoCharacter* MyPawn = Cast<AHackNSlashProtoCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AHackNSlashProtoPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
		else
		{
			RotateTowardCursor();
		}
	}
}

void AHackNSlashProtoPlayerController::RotateTowardCursor()
{
	if (AHackNSlashProtoCharacter* MyPawn = Cast<AHackNSlashProtoCharacter>(GetPawn()))
	{
		// Rotate the player toward the clicked location
		const FRotator rotation = UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(), MyPawn->GetCursorToWorld()->GetComponentLocation());
		const FRotator newRotationNoY = FRotator(0.f, rotation.Yaw, 0.f);
		MyPawn->SetActorRotation(newRotationNoY);
	}
}

UVMHealth* AHackNSlashProtoPlayerController::GetHealthVM()
{
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(GetPawn()))
	{
		if(UHealth* ActorHealth = Cast<UHealth>(player->GetComponentByClass(UHealth::StaticClass())))
		{
			return ActorHealth->HealthViewModel;
		}
	}

	return nullptr;
}

UVMMana* AHackNSlashProtoPlayerController::GetManaVM()
{
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(GetPawn()))
	{
		if(UMana* ActorMana = Cast<UMana>(player->GetComponentByClass(UMana::StaticClass())))
		{
			return ActorMana->ManaViewModel;
		}
	}

	return nullptr;
}

void AHackNSlashProtoPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AHackNSlashProtoPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
