// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

class UVMHealth;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthChangedSignature, float, Delta, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HACKNSLASHPROTO_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealth();

	// Sets default values for this component's properties
	virtual void InitializeComponent() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ApplyHealthChange(float Value);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHealthChangedSignature HealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDeathSignature Death;

	UPROPERTY(BlueprintReadWrite)
	UVMHealth* HealthViewModel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Current = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;
};
