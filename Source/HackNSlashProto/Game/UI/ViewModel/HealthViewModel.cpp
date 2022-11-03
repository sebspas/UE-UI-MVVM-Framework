#include "HealthViewModel.h"

#include "HackNSlashProto/Game/Component/Health.h"
#include "HackNSlashProto/Game/UI/ViewModelObject/HealthViewModelObject.h"


auto
	UHealthViewModel::
	Initialize(AActor* Actor)
-> void
{
	Super::Initialize(Actor);
	
	ViewModelObject = NewObject<UHealthViewModelObject>();
	View_ViewModelObject = NewObject<UHealthViewModelObject>();

	auto healthComponent = OwnerActor->FindComponentByClass<UHealth>();
	if(!healthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The health component is not present on %s when Initialize is called"), *OwnerActor->GetName());
		return;
	}

	healthComponent->HealthChanged.AddUniqueDynamic(this, &UHealthViewModel::OnHealthChanged);

	OnHealthChanged(0.f, healthComponent->Current, healthComponent->MaxHealth);
}

void UHealthViewModel::OnDestroy(AActor* Actor)
{
	auto healthComponent = Cast<UHealth>(OwnerActor->GetComponentByClass(UHealth::StaticClass()));
	if(!healthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The health component is not present on %s when Destroy is called"), *OwnerActor->GetName());
		return;
	}
	
	healthComponent->HealthChanged.RemoveDynamic(this, &UHealthViewModel::OnHealthChanged);
}

auto
	UHealthViewModel::OnHealthChanged(float Delta, float Current, float Max)
	-> void
{
	Set_Current(Current);
	Set_MaxHealth(Max);
}
