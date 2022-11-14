#pragma once
#include "HackNSlashProto/Core/MVVM/ViewModelObject/ViewModelObject.h"

#include "DamageNumberViewModelObject.generated.h"

USTRUCT(BlueprintType)
struct FDamageNumber
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool IsActivated = false;
	
	UPROPERTY(BlueprintReadOnly)
	float Value = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float LifeSpan = 0.7f;
};

UCLASS(BlueprintType)
class UDamageNumberViewModelObject : public UViewModelObject
{
	GENERATED_BODY()

public:
	UDamageNumberViewModelObject() {}

	UPROPERTY(BlueprintReadOnly)
	TArray<FDamageNumber> DamageNumbers;

	UPROPERTY(BlueprintReadOnly)
	FVector2D ActorScreenLocation;

	UPROPERTY(BlueprintReadOnly)
	float DeltaTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool IsVisibleOnScreen = false;
};
