#include "DamageNumberViewModel.h"

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
		CORE_LOGM(LogHackNSlashProto, "The health component is not present on %s when Initialize is called", *OwnerActor->GetName());
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
		CORE_LOGM(LogHackNSlashProto, "The health component is not present on %s when Destroy is called", *OwnerActor->GetName());
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
	auto ScreenActorLocation = DamageNumberViewModelObject->ActorScreenLocation;
	if(PlayerController)
	{
		bool IsVisible = PlayerController->ProjectWorldLocationToScreen(OwnerActor->GetActorLocation(), ScreenActorLocation, true);

		if(DamageNumberViewModelObject->IsVisibleOnScreen != IsVisible)
		{
			QueueVMObjectChange([IsVisible](UViewModelObject* ViewModelModelObject)
			{
				const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelModelObject);
				DamageNumberViewModelObject->IsVisibleOnScreen = IsVisible;
						
			}, {FName("IsVisibleOnScreen")});
		}
	}

	auto HasAnyActiveMarker = false;
	// Update the local VM LifeSpan and if necessary change the visibility of the appropriate number
	for (int i = 0; i < DamageNumberViewModelObject->DamageNumbers.Num(); ++i)
	{
		auto& DamageNumber = DamageNumberViewModelObject->DamageNumbers[i];
		if(DamageNumber.IsActive)
		{
			HasAnyActiveMarker = true;
			DamageNumber.LifeSpan-= DeltaSeconds;

			if(DamageNumber.LifeSpan <= 0)
			{
				QueueVMObjectChange([i](UViewModelObject* ViewModelModelObject)
				{
					const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelModelObject);
					DamageNumberViewModelObject->DamageNumbers[i].IsActive = false;
					DamageNumberViewModelObject->DamageNumbers[i].LifeSpan = 0.f; 
					
				}, {FName("IsActive")});
			}
		}
	}

	if(HasAnyActiveMarker)
	{
		QueueVMObjectChange([DeltaSeconds, ScreenActorLocation](UViewModelObject* ViewModelModelObject)
		{
			const auto DamageNumberViewModelObject = dynamic_cast<UDamageNumberViewModelObject*>(ViewModelModelObject);
			DamageNumberViewModelObject->ActorScreenLocation = ScreenActorLocation;
			DamageNumberViewModelObject->DeltaTime = DeltaSeconds;
						
		}, {FName("ActorScreenLocation")});
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
		SelectedDamageNumber.Value = Delta;
		SelectedDamageNumber.LifeSpan = 0.5f; // Could be data driven in the view
		SelectedDamageNumber.IsActive = true;
	}, {FName("DamageNumbers")});
}
