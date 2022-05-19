#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModelObject/ViewModelObject.h"

#include "HealthViewModelObject.generated.h"

UCLASS(BlueprintType)
class UHealthViewModelObject : public UViewModelObject
{
	GENERATED_BODY()

public:
	UHealthViewModelObject() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Current = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;
};
