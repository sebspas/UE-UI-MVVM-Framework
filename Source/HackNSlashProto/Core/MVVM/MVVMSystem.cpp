#include "MVVMSystem.h"

#include <imgui.h>
#include <string>
#include <ThirdParty/ImGuiLibrary/Private/imgui_internal.h>

#include "../System/ErrorDefine.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogUIMVVM);

void UMvvmSystem::Deinitialize()
{
	TSet<UView*> ViewsToUnregister;
	
	for(auto VMToViews : ViewModelsToViews)
	{
		ViewsToUnregister.Append(VMToViews.Value);
	}
	
	for (auto View : ViewsToUnregister)
	{
		UnRegisterView(View);
		View->SetIsRegisteredToMvvm(false);
	}
	
	Super::Deinitialize();
}

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

#if WITH_IMGUI
	ImGui::Begin("Test window");
	ImGui::SetWindowSize(ImVec2(500, 600));

	int id = 0;
	for (auto ViewModelTuple : ViewModelsTypeToViewModels)
	{
		auto ViewModelAndActorKey = ViewModelTuple.Key;

		ImGui::PushID(id++);
		std::string myString(TCHAR_TO_UTF8(*ViewModelAndActorKey.Value->GetName()));
		ImGui::Text("ActorID: %u \t ViewModelType: %s", ViewModelAndActorKey.Key, myString.c_str());
		ImGui::SameLine();

		
		if(ImGui::Button("Open"))
		{
			auto FoundGuiWindow = ImGuiWindowsOpened.Find(ViewModelAndActorKey);
			if(FoundGuiWindow)
			{
				ImGuiWindowsOpened[ViewModelAndActorKey] = !(*FoundGuiWindow);
			}
			else
			{
				ImGuiWindowsOpened.Add(ViewModelAndActorKey, true);	
			}
		}
		
		auto FoundGuiWindow = ImGuiWindowsOpened.Find(ViewModelAndActorKey);
		if(FoundGuiWindow != nullptr && *FoundGuiWindow)
		{
			ImGui::Begin("ViewModelObject");
			ImGui::SetWindowSize(ImVec2(400, 400));

			auto ViewModelValue = ViewModelTuple.Value;
			auto ViewModelObject = ViewModelValue->GetViewModelObject();
			auto InternalViewModelObject = ViewModelValue->GetInternalViewModelObject();

			for (TFieldIterator<FProperty> PropIt(ViewModelObject->GetClass()); PropIt; ++PropIt)
			{
				FProperty* Property = *PropIt;
				std::string myString2(TCHAR_TO_UTF8(*PropIt->GetName()));
				ImGui::Text("%s", myString2.c_str());
				
				if (PropIt->IsA(FBoolProperty::StaticClass()))
				{
					FBoolProperty *BoolProp = CastField<FBoolProperty>(Property);
					bool CurValue = BoolProp->GetPropertyValue_InContainer(ViewModelObject);
					ImGui::Checkbox("", &CurValue);
					BoolProp->SetPropertyValue_InContainer(ViewModelObject, CurValue);
					BoolProp->SetPropertyValue_InContainer(InternalViewModelObject, CurValue);
				}
				else if (PropIt->IsA(FFloatProperty::StaticClass()))
				{
					FFloatProperty *NumericProp = CastField<FFloatProperty>(Property);
					float CurValue = NumericProp->GetPropertyValue_InContainer(ViewModelObject);
					ImGui::DragFloat("", &CurValue);
					NumericProp->SetPropertyValue_InContainer(ViewModelObject, CurValue);
					NumericProp->SetPropertyValue_InContainer(InternalViewModelObject, CurValue);
				}
				else if (PropIt->IsA(FStructProperty::StaticClass()))
				{
					FStructProperty *StructProp = CastField<FStructProperty>(Property);
					std::string myString3(TCHAR_TO_UTF8(*Property->GetName()));
					ImGui::Text("Unsuported Type %s", myString3.c_str());
				}
			}
			
			ImGui::End();
		}
		ImGui::PopID();
	}
	
	ImGui::End();
#endif
}

auto
	UMvvmSystem::
	GetUMvvmSystem(UObject* Object)
	-> UMvvmSystem*
{
	const auto GameInstance = UGameplayStatics::GetGameInstance(Object);
	if(!GameInstance)
	{
		CORE_LOG(LogUIMVVM, TEXT("No GameInstance Found"));
		return nullptr;
	} 
	return GameInstance->GetSubsystem<UMvvmSystem>();

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

			auto newViewList = TArray<UView*>();
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
