#pragma once

#include "MVVMViewModelBase.h"

#include "UVMDamageNumbers.generated.h"

USTRUCT(BlueprintType)
struct FDamageNumberData
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
class UVMDamageNumbers : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UVMDamageNumbers();

	auto UpdateDamageNumber(int8 Index) -> void;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FDamageNumberData> DamageNumbers;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter)
	FVector2D ActorScreenLocation;

	FVector2D GetActorScreenLocation() const;
	void SetActorScreenLocation(const FVector2D& ActorScreenLocation);

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter)
	float DeltaTime = 0.f;
	
	float GetDeltaTime() const;
	void SetDeltaTime(float newDeltaTime);

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	bool IsVisibleOnScreen = false;
	
	bool GetIsVisibleOnScreen() const;
	void SetIsVisibleOnScreen(bool bIsVisibleOnScreen);

	UFUNCTION(BlueprintPure, FieldNotify)
	FDamageNumberData UpdatedDamageNumber() const
	{
		return DamageNumbers[IndexModified];
	}

private:
	int8 IndexModified = 0;
};
