#pragma once
#include "HackNSlashProto/Core/System/CoreSystem.h"
#include "View/View.h"
#include "ViewModel/ViewModel.h"

#include "MvvmSystem.generated.h"

UCLASS(BlueprintType)
class UMvvmSystem final : public UCoreSystem
{
public:
	GENERATED_BODY()

	UMvvmSystem() {}
	virtual ~UMvvmSystem() override = default;
	
	virtual void Update(float DeltaSeconds) override;

	auto RegisterView(UView* View) -> void;
	auto UnRegisterView(UView* View) -> void;

private:
	using ViewModelAndActorKey = TTuple<uint32, TSubclassOf<UViewModel>>;
	
	TMap<ViewModelAndActorKey, UViewModel*> ViewModelsTypeToViewModels;
	TMap<ViewModelAndActorKey, TArray<const UView*>> ViewModelsToViews;
};
