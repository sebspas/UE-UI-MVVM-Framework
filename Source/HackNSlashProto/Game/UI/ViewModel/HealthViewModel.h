#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"

#include "HealthViewModel.generated.h"

UCLASS()
class UHealthViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	virtual auto DiffViewModelObject() -> void override;
};
