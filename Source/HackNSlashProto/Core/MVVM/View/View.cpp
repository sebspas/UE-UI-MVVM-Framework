#include "View.h"

#include "HackNSlashProto/HackNSlashProtoGameMode.h"
#include "HackNSlashProto/Core/MVVM/MVVMSystem.h"
#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"
#include "Kismet/GameplayStatics.h"

bool UView::Initialize()
{
	const auto GameMode = dynamic_cast<AHackNSlashProtoGameMode*>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("No GameMode Found"));
		return UUserWidget::Initialize();
	}

	auto MVVMSystem = dynamic_cast<UMvvmSystem*>(GameMode->GetSystem(UMvvmSystem::StaticClass()));
	if(!MVVMSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No MvvmSystem Found"));
		return UUserWidget::Initialize();
	}

	MVVMSystem->RegisterView(this);
	
	return UUserWidget::Initialize();
}

void UView::RemoveFromParent()
{
	const auto GameMode = dynamic_cast<AHackNSlashProtoGameMode*>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("No GameMode Found"));
		return;
	}

	auto MVVMSystem = dynamic_cast<UMvvmSystem*>(GameMode->GetSystem(UMvvmSystem::StaticClass()));
	if(!MVVMSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No MvvmSystem Found"));
		return;
	}

	MVVMSystem->UnRegisterView(this);
	
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
			auto ViewModelPtr = ViewModelStruct.ViewModel.Get();
			if(!IsValid(ViewModelPtr))
			{
				return nullptr;
			}

			return ViewModelPtr;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("ViewModel of type %s wasn't found, make sure you requested it in the viewmodel array"), *ViewModelType->GetName());
	return nullptr;
}

auto
	UView::
	GetViewModelsRegistered() 
	-> TArray<FViewModelStruct>&
{
	return ViewModels;
}
