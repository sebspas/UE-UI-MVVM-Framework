#include "HealthViewModel.h"

#include "HackNSlashProto/Game/UI/ViewModelObject/HealthViewModelObject.h"


auto
	UHealthViewModel::
	Initalize()
-> void
{
	ViewModelObject = NewObject<UHealthViewModelObject>();
	View_ViewModelObject = NewObject<UHealthViewModelObject>();

	GetActor()
	
}

void UHealthViewModel::Update(float DeltaSeconds)
{
	auto healthViewModelObject = dynamic_cast<UHealthViewModelObject*>(ViewModelObject);

	if(healthViewModelObject->Current + 0.5 <= healthViewModelObject->MaxHealth)
	{
		QueueVMObjectChange([](UViewModelObject* ViewModelModelObject)
		{
			auto healthViewModelObject = dynamic_cast<UHealthViewModelObject*>(ViewModelModelObject);
			healthViewModelObject->Current += 0.5;
		}, FName("Current"));
	}
}
