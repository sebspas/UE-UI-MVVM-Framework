#include "DamageNumberComponent.h"

#include "Health.h"
#include "GameFramework/Actor.h"
#include "HackNSlashProto/HackNSlashProto.h"
#include "HackNSlashProto/Core/System/ErrorDefine.h"
#include "Kismet/GameplayStatics.h"

UDamageNumberComponent::UDamageNumberComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UDamageNumberComponent::InitializeComponent()
{
	Super::InitializeComponent();

	DamageNumbers = NewObject<UVMDamageNumbers>();

	const auto HealthComponent = GetOwner()->FindComponentByClass<UHealth>();
	if(!HealthComponent)
	{
		CORE_LOGM(LogHackNSlashProto, "The health component is not present on %s when Initialize is called", *GetOwner()->GetName());
		return;
	}
	HealthComponent->HealthChanged.AddUniqueDynamic(this, &UDamageNumberComponent::OnHealthChanged);
}

void UDamageNumberComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calculate the Actor owner position on the player screen and send it to the View
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetOuter(), 0);
	auto ScreenActorLocation = DamageNumbers->ActorScreenLocation;
	if(PlayerController)
	{
		const bool IsVisible = PlayerController->ProjectWorldLocationToScreen(GetOwner()->GetActorLocation(), ScreenActorLocation, true);
		DamageNumbers->SetIsVisibleOnScreen(IsVisible);
	}

	auto HasAnyActiveMarker = false;
	// Update the local VM LifeSpan and if necessary change the visibility of the appropriate number
	for (int i = 0; i < DamageNumbers->DamageNumbers.Num(); ++i)
	{
		auto& DamageNumber = DamageNumbers->DamageNumbers[i];
		if(DamageNumber.LifeSpan != 0.f)
		{
			HasAnyActiveMarker = true;
			DamageNumber.LifeSpan-= DeltaTime;
			DamageNumber.IsActivated = false;

			if(DamageNumber.LifeSpan <= 0)
			{
				DamageNumber.LifeSpan = 0.f;
				DamageNumbers->UpdateDamageNumber(i);
			}
		}
	}

	if(HasAnyActiveMarker)
	{
		DamageNumbers->SetActorScreenLocation(ScreenActorLocation);
		DamageNumbers->SetDeltaTime(DeltaTime);
	}
}

void UDamageNumberComponent::OnHealthChanged(float Delta, float Current, float Max)
{
	uint8 SelectedIndex = 0;
	for (int i = 0; i < DamageNumbers->DamageNumbers.Num(); ++i)
	{
		if(DamageNumbers->DamageNumbers[i].LifeSpan == 0.f)
		{
			SelectedIndex = i;
			break;
		}

		// Find next available slot that is going to end soon3
		if(DamageNumbers->DamageNumbers[SelectedIndex].LifeSpan > DamageNumbers->DamageNumbers[i].LifeSpan)
		{
			SelectedIndex = i;
		}
	}
		
	FDamageNumberData& SelectedDamageNumber = DamageNumbers->DamageNumbers[SelectedIndex];
	SelectedDamageNumber.Value = Delta;
	SelectedDamageNumber.LifeSpan = 0.7f; // Could be data driven in the view
	SelectedDamageNumber.IsActivated = true;

	DamageNumbers->UpdateDamageNumber(SelectedIndex);
}

