#include "UVMMana.h"

void UVMMana::SetCurrentMana(int32 newCurrentMana)
{
	if (CurrentMana != newCurrentMana)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentMana, newCurrentMana);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaAsPercentage);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaText);
	}
}

void UVMMana::SetMaxMana(int32 newMaxMana)
{
	if (MaxMana != newMaxMana)
	{
		UE_MVVM_SET_PROPERTY_VALUE(MaxMana, newMaxMana);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaAsPercentage);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaText);
	}
}
