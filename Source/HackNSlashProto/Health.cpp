// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealth::ApplyHealthChange(float Value)
{
    if(Current <= 0.f)
    {
		Death.Broadcast();
    } 
    else 
    {
    	Current += Value;
    	HealthChanged.Broadcast(Current, MaxHealth);
    }
}

// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

