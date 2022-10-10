#include "View.h"

#include "../MVVMSystem.h"
#include "../ViewModel/ViewModel.h"
#include "../../System/ErrorDefine.h"
#include "Kismet/GameplayStatics.h"

void UView::InitializeView(AActor* NewOwningActor)
{
	ensureMsgf(NewOwningActor, TEXT("Owner Actor is empty when calling InitializeView for view %s"), *GetName());
	OwningActor = NewOwningActor;
	
	const auto MVVMSystem = UMvvmSystem::GetUMvvmSystem(GetOuter());
	if(!MVVMSystem)
	{
		CORE_LOG(LogUIMVVM, TEXT("No MvvmSystem Found"));
		return;
	}

	MVVMSystem->RegisterView(this);
	IsRegisteredToMVVM = true;

	OnViewInitialized();
}

void UView::RemoveFromParent()
{
	if(!IsRegisteredToMVVM)
	{
		return;
	}
	
	const auto MVVMSystem = UMvvmSystem::GetUMvvmSystem(GetOuter());
	if(!MVVMSystem)
	{
		CORE_LOG(LogUIMVVM, TEXT("No MvvmSystem Found"));
		return;
	}

	MVVMSystem->UnRegisterView(this);
	IsRegisteredToMVVM = false;
	
	UUserWidget::RemoveFromParent();
}

auto
	UView::
	GetViewModelByType(TSubclassOf<UViewModel> ViewModelType) const
	-> UViewModel*
{
	for (auto ViewModelStruct : ViewModels)
	{
		if(ViewModelStruct.ViewModelType == ViewModelType)
		{
			const auto ViewModelPtr = ViewModelStruct.ViewModel.Get();
			if(!IsValid(ViewModelPtr))
			{
				return nullptr;
			}

			return ViewModelPtr;
		}
	}

	CORE_LOGM(LogTemp, "ViewModel of type %s wasn't found, make sure you requested it in the viewmodel array", *ViewModelType->GetName());
	return nullptr;
}

auto
	UView::
	GetViewModelsRegistered() 
	-> TArray<FViewModelStruct>&
{
	return ViewModels;
}

auto
	UView::
	SetIsRegisteredToMvvm(bool IsRegisteredToMvvm)
	-> void
{
	IsRegisteredToMVVM = IsRegisteredToMvvm;
}
