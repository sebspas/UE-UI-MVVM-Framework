// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mana.generated.h"

class UVMMana;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FManaChangedSignature, float, Delta, float, Current, float, Max);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HACKNSLASHPROTO_API UMana : public UActorComponent
{
	GENERATED_BODY()

public:
	UMana();

	// Sets default values for this component's properties
	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	auto HasEnoughMana(float Cost) const -> bool { return Current > Cost; }

	UFUNCTION(BlueprintCallable)
	void ApplyManaChange(float Value);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FManaChangedSignature ManaChanged;

	UPROPERTY(BlueprintReadWrite)
	UVMMana* ManaViewModel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Current = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RegenValue = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RegenDelay = 0.5f;

private:
	float ElapsedTime = 0.f;
};
