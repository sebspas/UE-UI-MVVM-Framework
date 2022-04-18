#include "ViewModel.h"

auto
	UViewModel::
	RegisterOnPropertyChanged(const FName& PropertyName, const FViewModelPropertyChanged& PropertyChanged)
	-> void
{
	auto& ValueFound = RegisteredPropertyMulticast.Add(PropertyName);
	ValueFound.Add(PropertyChanged);
}

auto
	UViewModel::
	UnRegisterOnPropertyChanged(const FName& PropertyName, const FViewModelPropertyChanged& PropertyChanged)
	-> void
{
	const auto ValueFound = RegisteredPropertyMulticast.Find(PropertyName);
	if(ValueFound)
	{
		ValueFound->Remove(PropertyChanged);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The property %s was never properly registered"), PropertyName);
	}
}

auto
	UViewModel::
	OnPropertyChangedEvent(const FName& PropertyName)
	-> void
{
	auto DelegateArrayPtr = RegisteredPropertyMulticast.Find(PropertyName);
	if(DelegateArrayPtr)
	{
		auto DelegateArray = *DelegateArrayPtr;
		for (auto& Delegate : DelegateArray)
		{
			Delegate.ExecuteIfBound();
		}
	}
}
