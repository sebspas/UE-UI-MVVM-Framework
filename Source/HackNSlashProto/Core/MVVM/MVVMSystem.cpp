#include "MVVMSystem.h"

void FMvvmSystem::Initialize()
{
	// Call the init on the different ViewModel
	[this]()
	{
		for (auto& viewModelTuple : ViewModelsTypeToViewModels)
		{
			viewModelTuple.Value.Initalize();
		}
	}();
}

void FMvvmSystem::Update(float DeltaSeconds)
{
	// Update the ViewModel that are registered here
	// This can be run in a thread so the Update only update the ViewModelObject in the ViewModel not the View
	[this, DeltaSeconds]()
	{
		for (auto& viewModelTuple : ViewModelsTypeToViewModels)
		{
			viewModelTuple.Value.Update(DeltaSeconds);
		}
	}();
	
	// Diff the view model of the view and the one of the viewmodel
	// Trigger the event related to the different property that changed in the view model
	// This needs to be done in the main thread
	[this, DeltaSeconds]()
	{
		for (auto& viewModelTuple : ViewModelsTypeToViewModels)
		{
			viewModelTuple.Value.DiffViewModelObject();
		}
	}();
}

auto
	FMvvmSystem::
	RegisterView(const UView* View)
	-> void
{
	for (auto ViewModelType : View->GetViewModelsRegistered())
	{
		/*auto& ViewList = ViewModelsToViews.Find(ViewModelType);
		ViewList.Add(View);

		auto& ViewModel = ViewModelsTypeToViewModels.Add(ViewModelType);
		View->*/
	}
}

auto
	FMvvmSystem::
	UnRegisterView(const UView* View)
	-> void
{
	
}
