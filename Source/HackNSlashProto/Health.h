// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedSignature, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACKNSLASHPROTO_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();

	UFUNCTION(BlueprintCallable)
	void ApplyHealthChange(float Value);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHealthChangedSignature HealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDeathSignature Death;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Current = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;
};
