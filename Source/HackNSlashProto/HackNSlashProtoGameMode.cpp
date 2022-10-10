// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HackNSlashProtoGameMode.h"

#include "HackNSlashProto.h"
#include "Core/MVVM/MVVMSystem.h"
#include "Core/System/ErrorDefine.h"
#include "Kismet/GameplayStatics.h"

AHackNSlashProtoGameMode::AHackNSlashProtoGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AHackNSlashProtoGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	const auto MVVMSystem = UMvvmSystem::GetUMvvmSystem(GetOuter());
	if(!MVVMSystem)
	{
		CORE_LOG(LogHackNSlashProto, TEXT("No MvvmSystem Found"));
		return;
	}

	Systems.Add(MVVMSystem);
}

void AHackNSlashProtoGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (auto system : Systems)
	{
		system->Update(DeltaSeconds);
	}
}
