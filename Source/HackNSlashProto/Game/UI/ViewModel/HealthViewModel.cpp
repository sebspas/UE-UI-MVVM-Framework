#include "HealthViewModel.h"

#include "HackNSlashProto/Game/UI/ViewModelObject/HealthViewModelObject.h"


auto
	UHealthViewModel::
	DiffViewModelObject()
	-> void
{
	DiffViewModelObjectTemplate<UHealthViewModel, FHealthViewModelObject>(this,
			Cast<FHealthViewModelObject>(View_ViewModelObject.Get()),
			Cast<FHealthViewModelObject>(ViewModelObject.Get()));
}
