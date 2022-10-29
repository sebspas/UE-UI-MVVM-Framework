﻿#include "DamageNumberViewModel.h"

#include "HackNSlashProto/HackNSlashProto.h"
#include "HackNSlashProto/Core/System/ErrorDefine.h"
#include "HackNSlashProto/Game/Component/Health.h"
#include "HackNSlashProto/Game/UI/ViewModelObject/DamageNumberViewModelObject.h"
#include "Kismet/GameplayStatics.h"

auto
	UDamageNumberViewModel::
	Initialize(AActor* Actor)
-> void
{
	Super::Initialize(Actor);
	
	ViewModelObject = NewObject<UDamageNumberViewModelObject>();
	View_ViewModelObject = NewObject<UDamageNumberViewModelObject>();

	const auto HealthComponent = OwnerActor->FindComponentByClass<UHealth>();
	if(!HealthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The health component is not present on %s when Initialize is called"), *OwnerActor->GetName());
		return;
	}

	HealthComponent->HealthChanged.AddUniqueDynamic(this, &UDamageNumberViewModel::OnHealthChanged);

	QueueVMObjectChange([](UViewModelObject* ViewModelModelObject)
	{
		const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelModelObject);
		DamageNumberViewModelObject->DamageNumbers.Init(FDamageNumber(),5);
	}, {FName("DamageNumbers")});
}

void UDamageNumberViewModel::Destroy(AActor* Actor)
{
	const auto healthComponent = Cast<UHealth>(OwnerActor->GetComponentByClass(UHealth::StaticClass()));
	if(!healthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The health component is not present on %s when Destroy is called"), *OwnerActor->GetName());
		return;
	}
	
	healthComponent->HealthChanged.RemoveDynamic(this, &UDamageNumberViewModel::OnHealthChanged);

	//TODO: Protect this call with a dummy pattern so the user don't forget them
	Super::Destroy(Actor);
}

auto
	UDamageNumberViewModel::
	Update(float DeltaSeconds)
	-> void
{
	const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelObject);
	
	// Calculate the Actor owner position on the player screen and send it to the View
	auto PlayerController = UGameplayStatics::GetPlayerController(GetOuter(), 0);
	if(PlayerController)
	{
		FVector2D ScreenActorLocation;
		bool IsVisible = PlayerController->ProjectWorldLocationToScreen(OwnerActor->GetActorLocation(), ScreenActorLocation, true);
		
		QueueVMObjectChange([DeltaSeconds, ScreenActorLocation, IsVisible](UViewModelObject* ViewModelModelObject)
		{
			const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelModelObject);
			DamageNumberViewModelObject->ActorScreenLocation = ScreenActorLocation;
			DamageNumberViewModelObject->IsVisibleOnScreen = IsVisible;
			DamageNumberViewModelObject->DeltaTime = IsVisible;
					
		}, {FName("ActorScreenLocation"), FName("IsVisibleOnScreen")});
		
	}

	// Update the local VM LifeSpan and if necessary change the visibility of the appropriate number
	
	uint8 Index = 0;
	for(auto& DamageNumber : DamageNumberViewModelObject->DamageNumbers)
	{
		if(DamageNumber.LifeSpan != 0)
		{
			DamageNumber.LifeSpan-= DeltaSeconds;

			if(DamageNumber.LifeSpan <= 0)
			{
				QueueVMObjectChange([Index](UViewModelObject* ViewModelModelObject)
				{
					const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelModelObject);
					DamageNumberViewModelObject->DamageNumbers[Index].IsActive = false;
					DamageNumberViewModelObject->DamageNumbers[Index].LifeSpan = 0.f; 
					
				}, {FName("IsActive")});
			}
		}
		Index++;
	}
}

auto
	UDamageNumberViewModel::OnHealthChanged(float Delta, float Current, float Max)
	-> void
{
	QueueVMObjectChange([Delta](UViewModelObject* ViewModelModelObject)
	{
		const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelModelObject);

		uint8 SelectedIndex = 0;
		for (int i = 0; i < DamageNumberViewModelObject->DamageNumbers.Num(); ++i)
		{
			if(!DamageNumberViewModelObject->DamageNumbers[i].IsActive)
			{
				SelectedIndex = i;
				break;
			}

			// Find next available slot that is going to end soon
			if(DamageNumberViewModelObject->DamageNumbers[SelectedIndex].LifeSpan > DamageNumberViewModelObject->DamageNumbers[i].LifeSpan)
			{
				SelectedIndex = i;
			}
		}
		
		FDamageNumber& SelectedDamageNumber = DamageNumberViewModelObject->DamageNumbers[SelectedIndex];
		SelectedDamageNumber.Index = SelectedIndex;
		SelectedDamageNumber.Value = Delta;
		SelectedDamageNumber.LifeSpan = 1.7f; // Could be data driven in the view
		SelectedDamageNumber.IsActive = true;

		DamageNumberViewModelObject->ChangeDamageNumberVisibility.Broadcast(SelectedIndex, true);
	}, {FName("DamageNumbers")});
}