#include "ViewModel.h"

#include "../MVVMSystem.h"
#include "../../System/ErrorDefine.h"

auto
	UViewModel::
	RegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChangedDelegate)
-> void
{
	const auto PropertyChangeListener = RegisteredPropertyMulticast.Find(PropertyName);
	if(PropertyChangeListener)
	{
		PropertyChangeListener->Add(PropertyChangedDelegate);
	}
	else
	{
		auto& ValueFound = RegisteredPropertyMulticast.Add(PropertyName);
		ValueFound.Add(PropertyChangedDelegate);
	}
}

auto
	UViewModel::
	UnRegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChangedDelegate)
-> void
{
	const auto ValueFound = RegisteredPropertyMulticast.Find(PropertyName);
	if(ValueFound)
	{
		ValueFound->Remove(PropertyChangedDelegate);
	}
	else
	{
		CORE_LOGM(LogUIMVVM, "The property %s was never properly registered", *PropertyName.ToString());
	}
}

auto
	UViewModel::
	OnPropertyChangedEvent(const FName& PropertyName)
	-> void
{
	const auto DelegateArrayPtr = RegisteredPropertyMulticast.Find(PropertyName);
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
	const SPropertiesChange NewPropertyChange = {{PropertyChange}, LambdaChange};
	ViewModelToViewQueue.Enqueue(NewPropertyChange);
}

auto
	UViewModel::QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const TArray<FName>& PropertiesChanged)
	-> void
{
	const SPropertiesChange NewPropertiesChanges = {PropertiesChanged, LambdaChange};
	ViewModelToViewQueue.Enqueue(NewPropertiesChanges);
}

void UViewModel::Initialize(AActor* Actor)
{
	OwnerActor = Actor;
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
