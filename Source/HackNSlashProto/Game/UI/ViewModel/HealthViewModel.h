#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"

#include "HealthViewModel.generated.h"

UCLASS(BlueprintType)
class UHealthViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	virtual auto Initialize(AActor* OwnerActor) -> void override;
	virtual auto Destroy(AActor* OwnerActor) -> void override;

	UFUNCTION()
	void OnHealthChanged(float Delta, float Current, float Max);
};
