#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModelObject/ViewModelObject.h"

#include "HealthViewModelObject.generated.h"

USTRUCT(BlueprintType)
struct FHealthViewModelObject : public FViewModelObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Current = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;
};
