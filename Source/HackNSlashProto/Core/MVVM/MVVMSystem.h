#pragma once
#include "HackNSlashProto/Core/System/ISystem.h"
#include "View/View.h"
#include "ViewModel/ViewModel.h"

class FMvvmSystem final : public ISystem
{
public:
	virtual void Initialize() override;
	virtual void Update(float DeltaSeconds) override;

	auto RegisterView(const UView* View) -> void;
	auto UnRegisterView(const UView* View) -> void;

private:
	TMap<TSubclassOf<UViewModel>, UViewModel> ViewModelsTypeToViewModels;
	TMap<TSubclassOf<UViewModel>, TArray<const UView*>> ViewModelsToViews;
};
