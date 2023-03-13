// Fill out your copyright notice in the Description page of Project Settings.
#include "Mana.h"

#include "../UI/ViewModel/UVMMana.h"
#include "HackNSlashProto/Game/UI/ViewModel/UVMMana.h"

UMana::UMana()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

// Sets default values for this component's properties
void UMana::InitializeComponent()
{
	Super::InitializeComponent();

	ManaViewModel = NewObject<UVMMana>();

	// Notify the defaults values of Current and MaxMana
	ManaViewModel->SetCurrentMana(Current);
	ManaViewModel->SetMaxMana(MaxMana);
}

void UMana::ApplyManaChange(float Value)
{
    Current += Value;
	
    if(Current <= 0.f)
    {
    	Current = 0.f;
    }

	ManaViewModel->SetCurrentMana(Current);
	ManaViewModel->SetMaxMana(MaxMana);
}

// Called every frame
void UMana::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Current < MaxMana)
	{
		ElapsedTime += DeltaTime;
		if(ElapsedTime >= RegenDelay)
		{
			Current += RegenValue;
			ManaViewModel->SetCurrentMana(Current);
			
			ElapsedTime = 0.f;
		}
	}
}

