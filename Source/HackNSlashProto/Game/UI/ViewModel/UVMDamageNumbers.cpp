#include "UVMDamageNumbers.h"

UVMDamageNumbers::UVMDamageNumbers()
{
	DamageNumbers.Init(FDamageNumberData(), 5);
}

auto UVMDamageNumbers::UpdateDamageNumber(int8 Index) -> void
{
	IndexModified = Index;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(UpdatedDamageNumber);
}

FVector2D UVMDamageNumbers::GetActorScreenLocation() const
{
	return ActorScreenLocation;
}

void UVMDamageNumbers::SetActorScreenLocation(const FVector2D& newActorScreenLocation)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActorScreenLocation, newActorScreenLocation);
}

float UVMDamageNumbers::GetDeltaTime() const
{
	return DeltaTime;
}

void UVMDamageNumbers::SetDeltaTime(float newDeltaTime)
{
	UE_MVVM_SET_PROPERTY_VALUE(DeltaTime, newDeltaTime);
}

bool UVMDamageNumbers::GetIsVisibleOnScreen() const
{
	return IsVisibleOnScreen;
}

void UVMDamageNumbers::SetIsVisibleOnScreen(bool bIsVisibleOnScreen)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsVisibleOnScreen, bIsVisibleOnScreen);
}



