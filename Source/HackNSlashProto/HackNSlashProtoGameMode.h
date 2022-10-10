// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HackNSlashProtoGameMode.generated.h"

class IUpdatableSubSystem;

UCLASS(minimalapi)
class AHackNSlashProtoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHackNSlashProtoGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void Tick(float DeltaSeconds) override;

private:
	TArray<IUpdatableSubSystem*> Systems;
};



