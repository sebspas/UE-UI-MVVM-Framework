#include "ViewModel.h"

#include "HackNSlashProto/Core/MVVM/MVVMSystem.h"

auto
	UViewModel::
	RegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChanged)
-> void
{
	auto& ValueFound = RegisteredPropertyMulticast.Add(PropertyName);
	ValueFound.Add(PropertyChanged);
}

auto
	UViewModel::
	UnRegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChanged)
-> void
{
	const auto ValueFound = RegisteredPropertyMulticast.Find(PropertyName);
	if(ValueFound)
	{
		ValueFound->Remove(PropertyChanged);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The property %s was never properly registered"), *PropertyName.ToString());
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

auto
	UViewModel::QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const FName& PropertyChange)
	-> void
{
	SPropertiesChange newPropertyChange = {{PropertyChange}, LambdaChange};
	ViewModelToViewQueue.Enqueue(newPropertyChange);
}

auto
	UViewModel::QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const TArray<FName>& PropertiesChanged)
	-> void
{
	SPropertiesChange newPropertiesChanges = {PropertiesChanged, LambdaChange};
	ViewModelToViewQueue.Enqueue(newPropertiesChanges);
}

auto
	UViewModel::
	ProcessChanges()
	-> void
{
	while (!ViewModelToViewQueue.IsEmpty())
	{
		const SPropertiesChange* QueuedChanged = ViewModelToViewQueue.Peek();
		
		// Update both ViewModelObject to the new Value
		QueuedChanged->ChangeLambda(ViewModelObject);
		QueuedChanged->ChangeLambda(View_ViewModelObject);

		// Inform that the property has just changed
		for (auto propertyName : QueuedChanged->PropertiesChanged)
		{
			OnPropertyChangedEvent(propertyName);
		}

		ViewModelToViewQueue.Pop();
	}
}