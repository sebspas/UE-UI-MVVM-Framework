#include "HealthViewModel.h"

#include "HackNSlashProto/Game/UI/ViewModelObject/HealthViewModelObject.h"


auto
	UHealthViewModel::
	Initalize()
-> void
{
	ViewModelObject = NewObject<UHealthViewModelObject>();
	View_ViewModelObject = NewObject<UHealthViewModelObject>();
}

void UHealthViewModel::Update(float DeltaSeconds)
{
	auto healthViewModelObject = dynamic_cast<UHealthViewModelObject*>(ViewModelObject);

	if(healthViewModelObject->Current + 0.5 <= healthViewModelObject->MaxHealth)
	{
		healthViewModelObject->Current += 0.5;
	}
}

auto
	UHealthViewModel::
	DiffViewModelObject()
	-> void
{
	DiffViewModelObjectTemplate<UHealthViewModel, UHealthViewModelObject>(this,
			(UHealthViewModelObject*)View_ViewModelObject,
			(UHealthViewModelObject*)ViewModelObject);
}
