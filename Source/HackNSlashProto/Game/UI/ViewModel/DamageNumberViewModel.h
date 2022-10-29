#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"

#include "DamageNumberViewModel.generated.h"

UCLASS(BlueprintType)
class UDamageNumberViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	virtual auto Initialize(AActor* Actor) -> void override;
	virtual auto Destroy(AActor* Actor) -> void override;
	virtual auto Update(float DeltaSeconds) -> void override;

	UFUNCTION()
	void OnHealthChanged(float Delta, float Current, float Max);
};
