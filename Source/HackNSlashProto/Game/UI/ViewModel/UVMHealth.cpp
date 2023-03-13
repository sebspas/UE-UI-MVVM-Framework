#include "UVMHealth.h"

void UVMHealth::SetCurrentHealth(int32 newCurrentHealth)
{
	if (CurrentHealth != newCurrentHealth)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, newCurrentHealth);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthText);
	}
}

void UVMHealth::SetMaxHealth(int32 newMaxHealth)
{
	if (MaxHealth != newMaxHealth)
	{
		UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, newMaxHealth);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthText);
	}
}
