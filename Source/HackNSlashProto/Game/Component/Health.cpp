// Fill out your copyright notice in the Description page of Project Settings.
#include "Health.h"

#include "HackNSlashProto/Game/UI/ViewModel/UVMHealth.h"

UHealth::UHealth()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

// Sets default values for this component's properties
void UHealth::InitializeComponent()
{
	Super::InitializeComponent();

	HealthViewModel = NewObject<UVMHealth>();

	// Notify the defaults values of Current and MaxHealth
	HealthViewModel->SetCurrentHealth(Current);
	HealthViewModel->SetMaxHealth(MaxHealth);
}

void UHealth::ApplyHealthChange(float Value)
{
    Current += Value;
	
    if(Current <= 0.f)
    {
    	Current = 0.f;
    	HealthViewModel->SetCurrentHealth(Current);
		Death.Broadcast();
    	return;
    }

	HealthViewModel->SetCurrentHealth(Current);
	HealthViewModel->SetMaxHealth(MaxHealth);

	HealthChanged.Broadcast(Value, Current, MaxHealth);
}

// Called every frame
void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

