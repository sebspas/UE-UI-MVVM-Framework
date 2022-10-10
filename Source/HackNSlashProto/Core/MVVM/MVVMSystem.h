﻿#pragma once

#include "../System/IUpdatableSubSystem.h"
#include "View/View.h"
#include "ViewModel/ViewModel.h"

#include "MvvmSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUIMVVM, Log, All);

UCLASS(BlueprintType)
class UMvvmSystem final : public UGameInstanceSubsystem, public IUpdatableSubSystem
{
public:
	GENERATED_BODY()

	UMvvmSystem() {}
	virtual ~UMvvmSystem() override = default;

	virtual void Deinitialize() override;
	
	virtual void Update(float DeltaSeconds) override;

	static UMvvmSystem* GetUMvvmSystem(UObject* Object);

	auto RegisterView(UView* View) -> void;
	auto UnRegisterView(UView* View) -> void;

private:
	using ViewModelAndActorKey = TTuple<uint32, TSubclassOf<UViewModel>>;
	
	TMap<ViewModelAndActorKey, UViewModel*> ViewModelsTypeToViewModels;
	TMap<ViewModelAndActorKey, TArray<UView*>> ViewModelsToViews;
};
