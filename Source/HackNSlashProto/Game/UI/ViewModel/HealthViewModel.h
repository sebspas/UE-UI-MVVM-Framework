#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"
#include "HackNSlashProto/Game/UI/ViewModelObject/HealthViewModelObject.h"

#include "HealthViewModel.generated.h"

UCLASS(BlueprintType)
class UHealthViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	virtual auto Initialize(AActor* Actor) -> void override;
	virtual auto OnDestroy(AActor* Actor) -> void override;

	UFUNCTION()
	void OnHealthChanged(float Delta, float Current, float Max);

	GENERATE_UI_SETTERS(UHealthViewModelObject, float, Current);
	GENERATE_UI_SETTERS(UHealthViewModelObject, float, MaxHealth);
};
