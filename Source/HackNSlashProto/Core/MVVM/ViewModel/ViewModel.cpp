#include "ViewModel.h"

#include "../MVVMSystem.h"
#include "../../System/ErrorDefine.h"

auto
	UViewModel::
	RegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChangedDelegate)
-> void
{
	if(const auto PropertyChangeListener = RegisteredPropertyMulticast.Find(PropertyName))
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
	if(const auto ValueFound = RegisteredPropertyMulticast.Find(PropertyName))
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
	if(const auto DelegateArrayPtr = RegisteredPropertyMulticast.Find(PropertyName))
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
	const FPropertiesChange NewPropertyChange = {{PropertyChange}, LambdaChange};
	ViewModelToViewQueue.Enqueue(NewPropertyChange);
}

auto
	UViewModel::QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const TArray<FName>& PropertiesChanged)
	-> void
{
	const FPropertiesChange NewPropertiesChanges = {PropertiesChanged, LambdaChange};
	ViewModelToViewQueue.Enqueue(NewPropertiesChanges);
}

void UViewModel::Initialize(AActor* Actor)
{
	OwnerActor = Actor;
}

void UViewModel::Destroy(AActor* Actor)
{
	OnDestroy(Actor);
}

auto
	UViewModel::
	ProcessChanges()
	-> void
{
	while (!ViewModelToViewQueue.IsEmpty())
	{
		FPropertiesChange QueuedChange;
		ViewModelToViewQueue.Dequeue(QueuedChange);
		
		// Update both ViewModelObject to the new Value
		QueuedChange.ChangeLambda(ViewModelObject);
		QueuedChange.ChangeLambda(View_ViewModelObject);

		// Inform that the property has just changed
		for (auto PropertyName : QueuedChange.PropertiesChanged)
		{
			OnPropertyChangedEvent(PropertyName);
		}

		ViewModelToViewQueue.Pop();
	}
}
