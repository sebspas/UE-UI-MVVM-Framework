#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"

#include "HealthViewModel.generated.h"

UCLASS(BlueprintType)
class UHealthViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	virtual auto Initialize(AActor* Actor) -> void override;
	virtual auto Destroy(AActor* Actor) -> void override;

	UFUNCTION()
	void OnHealthChanged(float Delta, float Current, float Max);
};
