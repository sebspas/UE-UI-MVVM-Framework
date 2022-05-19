// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HackNSlashProtoGameMode.h"

AHackNSlashProtoGameMode::AHackNSlashProtoGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AHackNSlashProtoGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (auto systemInfos : GameSystemsInfos)
	{
		auto newSystem = NewObject<UCoreSystem>(this, systemInfos.SystemType);

		//TODO Move this to another place
		newSystem->Initialize(systemInfos);
		
		Systems.Add(newSystem);
	}
}

void AHackNSlashProtoGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (auto system : Systems)
	{
		system->Update(DeltaSeconds);
	}
}

auto
	AHackNSlashProtoGameMode::
	GetSystem(TSubclassOf<UCoreSystem> SystemType) const
	-> UCoreSystem*
{
	for (auto system : Systems)
	{
		if(SystemType == system->GetClass())
		{
			return system;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("The system %s was never properly registered"), *SystemType->GetName());
	return nullptr;
}
