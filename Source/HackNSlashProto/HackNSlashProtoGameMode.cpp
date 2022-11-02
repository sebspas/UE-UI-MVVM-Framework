// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HackNSlashProtoGameMode.h"

#include <imgui.h>

#include "HackNSlashProto.h"
#include "Core/MVVM/MVVMSystem.h"
#include "Core/System/ErrorDefine.h"
#include "Core/System/ImGuiHelpers.h"
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

#if WITH_IMGUI
	ImGuiWindowsOpened.Add(MVVMSystem->GetSystemName());
#endif
}

void AHackNSlashProtoGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (auto system : Systems)
	{
		system->Update(DeltaSeconds);

#if WITH_IMGUI
		ImGuiTick();
#endif
	}
}

#if WITH_IMGUI
auto
	AHackNSlashProtoGameMode::
	ImGuiTick()
	-> void
{
	ImGui::BeginMainMenuBar();

	if(ImGui::BeginMenu("Systems"))
	{
		for (auto system : Systems)
		{
			ImGui::MenuItem(ImGui::ToConstCharPtr(system->GetSystemName()), nullptr, &ImGuiWindowsOpened[system->GetSystemName()]);
		}
		ImGui::EndMenu();
	}
	
	for (auto system : Systems)
	{
		system->UpdateImGuiSystemWindow(ImGuiWindowsOpened[system->GetSystemName()]);
	}
	
	ImGui::EndMainMenuBar();
}
#endif
