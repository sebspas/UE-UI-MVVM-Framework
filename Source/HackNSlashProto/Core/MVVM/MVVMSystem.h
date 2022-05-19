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
	
	virtual void Initialize(const FSystemInfos& infos) override;
	virtual void Update(float DeltaSeconds) override;

	auto RegisterView(UView* View) -> void;
	auto UnRegisterView(UView* View) -> void;

private:
	TMap<TSubclassOf<UViewModel>, UViewModel*> ViewModelsTypeToViewModels;
	TMap<TSubclassOf<UViewModel>, TArray<const UView*>> ViewModelsToViews;
};
