#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"

#include "HealthViewModel.generated.h"

UCLASS(BlueprintType)
class UHealthViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	auto Initalize(AActor* OwnerActor) -> void override;
	auto Destroy(AActor* OwnerActor) -> void override;

	UFUNCTION()
	void OnHealthChanged(float Delta, float Current, float Max);
};
