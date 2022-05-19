#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"

#include "HealthViewModel.generated.h"

UCLASS(BlueprintType)
class UHealthViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	virtual auto Initalize() -> void override;
	virtual auto Update(float DeltaSeconds) -> void;
	
	virtual auto DiffViewModelObject() -> void override;
};
