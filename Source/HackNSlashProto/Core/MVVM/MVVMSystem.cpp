#include "MVVMSystem.h"

#include "HackNSlashProto/Core/System/ErrorDefine.h"

DEFINE_LOG_CATEGORY(LogUIMVVM);

void UMvvmSystem::Update(float DeltaSeconds)
{
	// Update the ViewModel that are registered here
	// This can be run in a thread so the Update only update the ViewModelObject in the ViewModel not the View
	[this, DeltaSeconds]()
	{
		for (const auto& ViewModelTuple : ViewModelsTypeToViewModels)
		{
			ViewModelTuple.Value->Update(DeltaSeconds);
		}
	}();
	
	// Process the changes queue and forward update notification
	// Trigger the event related to the different property that changed in the view model
	// This needs to be done in the main thread
	[this]()
	{
		for (const auto& ViewModelTuple : ViewModelsTypeToViewModels)
		{
			ViewModelTuple.Value->ProcessChanges();
		}
	}();
}

auto
	UMvvmSystem::
	RegisterView(UView* View)
	-> void
{
	const auto OwnerActor = View->GetOwningActor();
	ensureMsgf(OwnerActor, TEXT("Owner Actor is empty when registering view %s"), *View->GetName());
	
	for (auto& ViewModelsRegistered : View->GetViewModelsRegistered())
	{
		auto ViewModelType = ViewModelsRegistered.ViewModelType;
		auto ViewModelActorInfos = ViewModelAndActorKey{OwnerActor->GetUniqueID(), ViewModelType};

		const auto ViewModel = ViewModelsTypeToViewModels.Find(ViewModelActorInfos);
		if(ViewModel)
		{
			// ViewModel exist simply check the view and update the pointer in it for this viewModel
			const auto viewList = ViewModelsToViews.Find(ViewModelActorInfos);
			if(!viewList)
			{
				CORE_LOG(LogUIMVVM, TEXT("No Views where registered but a ViewModel still exist this shouldn't happens."));
				return;
			}

			viewList->Add(View);

			ViewModelsRegistered.ViewModel = *ViewModel;
		}
		else
		{
			// No ViewModel exist yet for this type, create a new viewModel add it and then create the ViewList
			auto newViewModel = NewObject<UViewModel>(this, ViewModelType);

			newViewModel->Initialize(OwnerActor);

			ViewModelsRegistered.ViewModel = newViewModel;
			ViewModelsTypeToViewModels.Add(ViewModelActorInfos, newViewModel);

			auto newViewList = TArray<const UView*>();
			newViewList.Add(View);

			ViewModelsToViews.Add(ViewModelActorInfos, newViewList);
		}
	}
}

auto
	UMvvmSystem::
	UnRegisterView(UView* View)
	-> void
{
	const auto OwnerActor = View->GetOwningActor();
	ensureMsgf(OwnerActor, TEXT("Owner Actor is empty when unregistering view %s"), *View->GetName());
	
	for (const auto ViewModelsRegistered : View->GetViewModelsRegistered())
	{
		auto ViewModelType = ViewModelsRegistered.ViewModelType;
		auto ViewModelActorInfos = ViewModelAndActorKey{OwnerActor->GetUniqueID(), ViewModelType};

		const auto ViewModel = ViewModelsTypeToViewModels.Find(ViewModelActorInfos);
		if(ViewModel)
		{
			(*ViewModel)->Destroy(OwnerActor);
			
			const auto viewList = ViewModelsToViews.Find(ViewModelActorInfos);
			if(!viewList)
			{
				CORE_LOG(LogUIMVVM, TEXT("No Views where registered but a ViewModel still exist this shouldn't happens."));
				return;
			}

			viewList->Remove(View);

			// If no views are still using this ViewModel we can clean up everything
			if(viewList->IsEmpty())
			{
				ViewModelsTypeToViewModels.Remove(ViewModelActorInfos);
				ViewModelsToViews.Remove(ViewModelActorInfos);
			}
		}
		else
		{
			CORE_LOG(LogUIMVVM, TEXT("No Views where registered but a ViewModel still exist this shouldn't happens."));
		}
	}
}
