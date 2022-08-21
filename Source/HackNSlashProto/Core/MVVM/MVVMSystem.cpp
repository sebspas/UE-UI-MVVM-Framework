#include "MVVMSystem.h"

void UMvvmSystem::Initialize(const FSystemInfos& infos)
{
	UCoreSystem::Initialize(infos);
	
	// Call the init on the different ViewModel
	[this]()
	{
		for (auto& viewModelTuple : ViewModelsTypeToViewModels)
		{
			viewModelTuple.Value->Initalize();
		}
	}();
}

void UMvvmSystem::Update(float DeltaSeconds)
{
	// Update the ViewModel that are registered here
	// This can be run in a thread so the Update only update the ViewModelObject in the ViewModel not the View
	[this, DeltaSeconds]()
	{
		for (auto& viewModelTuple : ViewModelsTypeToViewModels)
		{
			viewModelTuple.Value->Update(DeltaSeconds);
		}
	}();
	
	// Process the changes queue and forward update notification
	// Trigger the event related to the different property that changed in the view model
	// This needs to be done in the main thread
	[this, DeltaSeconds]()
	{
		for (auto& viewModelTuple : ViewModelsTypeToViewModels)
		{
			viewModelTuple.Value->ProcessChanges();
		}
	}();
}

auto
	UMvvmSystem::
	RegisterView(UView* View)
	-> void
{
	for (auto& ViewModelsRegistered : View->GetViewModelsRegistered())
	{
		auto viewModelType = ViewModelsRegistered.ViewModelType;

		auto viewModel = ViewModelsTypeToViewModels.Find(viewModelType);
		if(viewModel)
		{
			// ViewModel exist simply check the view and update the pointer in it for this viewModel

			const auto viewList = ViewModelsToViews.Find(viewModelType);
			if(!viewList)
			{
				UE_LOG(LogTemp, Error, TEXT("No Views where registered but a ViewModel still exist this shouldn't happens."));
				return;
			}

			viewList->Add(View);

			ViewModelsRegistered.ViewModel = *viewModel;
		}
		else
		{
			// No ViewModel exist yet for this type, create a new viewModel add it and then create the ViewList
			auto newViewModel = NewObject<UViewModel>(this, viewModelType);

			//TODO see if Initializing here is an issue but shoudn't be
			newViewModel->Initalize();

			ViewModelsRegistered.ViewModel = newViewModel;
			ViewModelsTypeToViewModels.Add(viewModelType, newViewModel);

			auto newViewList = TArray<const UView*>();
			newViewList.Add(View);

			ViewModelsToViews.Add(viewModelType, newViewList);
		}
	}
}

auto
	UMvvmSystem::
	UnRegisterView(UView* View)
	-> void
{
	for (auto ViewModelsRegistered : View->GetViewModelsRegistered())
	{
		auto viewModelType = ViewModelsRegistered.ViewModelType;

		auto viewModel = ViewModelsTypeToViewModels.Find(viewModelType);
		if(viewModel)
		{
			const auto viewList = ViewModelsToViews.Find(viewModelType);
			if(!viewList)
			{
				UE_LOG(LogTemp, Error, TEXT("No Views where registered but a ViewModel still exist this shouldn't happens."));
				return;
			}

			viewList->Remove(View);

			// If no views are still using this ViewModel we can clean up everything
			if(viewList->IsEmpty())
			{
				ViewModelsTypeToViewModels.Remove(viewModelType);
				ViewModelsToViews.Remove(viewModelType);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Views where registered but a ViewModel still exist this shouldn't happens."));
		}
	}
}
