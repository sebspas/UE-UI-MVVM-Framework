#include "MVVMSystem.h"

#include <imgui.h>
#include <string>
#include <ThirdParty/ImGuiLibrary/Private/imgui_internal.h>

#include "../System/ErrorDefine.h"
#include "HackNSlashProto/Core/System/ImGuiHelpers.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogUIMVVM);

UMvvmSystem::UMvvmSystem()
{
	Name = "MVVMSystem";
}

auto
	UMvvmSystem::
	Deinitialize()
	-> void
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
}

#if WITH_IMGUI
void UMvvmSystem::UpdateImGuiSystemWindow(bool& IsWindowOpen)
{
	if(!IsWindowOpen)
	{
		return;
	}
	
	ImGui::SetNextWindowSize(ImVec2(500, 600));
	ImGui::Begin("MVVM System", &IsWindowOpen);
	int ID = 0;
	for (const auto ViewModelTuple : ViewModelsTypeToViewModels)
	{
		auto ViewModelAndActorKey = ViewModelTuple.Key;

		ImGui::PushID(ID++);
		ImGui::Text("ActorID: %u \t ViewModelType: %s", ViewModelAndActorKey.Key, ImGui::ToConstCharPtr(ViewModelAndActorKey.Value->GetName()));
		ImGui::SameLine();
		
		if(ImGui::Button("Open"))
		{
			if(const auto FoundGuiWindow = ImGuiWindowsOpened.Find(ViewModelAndActorKey))
			{
				ImGuiWindowsOpened[ViewModelAndActorKey] = !(*FoundGuiWindow);
			}
			else
			{
				ImGuiWindowsOpened.Add(ViewModelAndActorKey, true);	
			}
		}

		const auto FoundGuiWindow = ImGuiWindowsOpened.Find(ViewModelAndActorKey);
		if(FoundGuiWindow != nullptr && *FoundGuiWindow)
		{
			const auto ViewModelValue = ViewModelTuple.Value;
			const auto ViewModelObject = ViewModelValue->GetViewModelObject();
			const auto InternalViewModelObject = ViewModelValue->GetInternalViewModelObject();

			auto string = FString::Printf(TEXT("%s - %u"), *ViewModelObject->GetName(), ViewModelAndActorKey.Key);
			ImGui::Begin(ImGui::ToConstCharPtr(string), FoundGuiWindow);
			ImGui::SetWindowSize(ImVec2(400, 400), ImGuiCond_Once);
			

			int Prop_Imgui_Id = 0;
			for (TFieldIterator<FProperty> PropIt(ViewModelObject->GetClass()); PropIt; ++PropIt)
			{
				FProperty* Property = *PropIt;
				auto PropertyName = FName(Property->GetName());
				ImGui::Text("%s", ImGui::ToConstCharPtr(PropIt->GetName()));
				ImGui::SameLine();
				
				if (PropIt->IsA(FBoolProperty::StaticClass()))
				{
					const FBoolProperty *BoolProp = CastField<FBoolProperty>(Property);
					bool CurValue = BoolProp->GetPropertyValue_InContainer(ViewModelObject);
					ImGui::PushID(Prop_Imgui_Id++);
					if(ImGui::Checkbox(ImGui::ToConstCharPtr(PropertyName), &CurValue))
					{
						ViewModelValue->QueueVMObjectChange([NewValue = CurValue, PropertyName](UViewModelObject* ViewModelModelObject)
						{
							auto* Prop = CastField<FBoolProperty>(ViewModelModelObject->GetClass()->FindPropertyByName(PropertyName));
							Prop->SetPropertyValue_InContainer(ViewModelModelObject, NewValue);
						}, PropertyName);
					}
					ImGui::PopID();
				}
				else if (PropIt->IsA(FFloatProperty::StaticClass()))
				{
					const FFloatProperty *NumericProp = CastField<FFloatProperty>(Property);
					float CurValue = NumericProp->GetPropertyValue_InContainer(ViewModelObject);
					ImGui::PushID(Prop_Imgui_Id++);
					if(ImGui::DragFloat(ImGui::ToConstCharPtr(PropertyName), &CurValue))
					{
						ViewModelValue->QueueVMObjectChange([NewValue = CurValue, PropertyName](UViewModelObject* ViewModelModelObject)
						{
							auto* Prop = CastField<FFloatProperty>(ViewModelModelObject->GetClass()->FindPropertyByName(PropertyName));
							Prop->SetPropertyValue_InContainer(ViewModelModelObject, NewValue);
						}, PropertyName);
					}
					ImGui::PopID();
				}
				else
				{
					if(Property->GetCPPType() == "FVector2D")
					{
						const FVector2D* Vector2D = Property->ContainerPtrToValuePtr<FVector2D>(ViewModelObject);
						float Test[2] = {static_cast<float>(Vector2D->X), static_cast<float>(Vector2D->Y)};
						
						ImGui::PushID(Prop_Imgui_Id++);
						if(ImGui::InputFloat2(ImGui::ToConstCharPtr(PropertyName), Test))
						{
							ViewModelValue->QueueVMObjectChange([NewValue = Test, PropertyName](UViewModelObject* ViewModelModelObject)
							{
								auto* Prop = CastField<FProperty>(ViewModelModelObject->GetClass()->FindPropertyByName(PropertyName));
								FVector2D* Vector2D = Prop->ContainerPtrToValuePtr<FVector2D>(ViewModelModelObject);
								Vector2D->Set(NewValue[0], NewValue[1]);
							}, PropertyName);
						}
					}
					else
					{
						FString Output = TEXT("");
						Property->ExportTextItem(Output, Property->ContainerPtrToValuePtr<uint8>(ViewModelObject), nullptr, ViewModelObject, 0);
						ImGui::Text("%s", ImGui::ToConstCharPtr(Output));
					}
				}
			}
			ImGui::End();
		}
		ImGui::PopID();
	}
	ImGui::End();
}
#endif

auto
	UMvvmSystem::
	GetUMvvmSystem(const UObject* Object)
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

		if(const auto ViewModel = ViewModelsTypeToViewModels.Find(ViewModelActorInfos))
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
			auto NewViewModel = NewObject<UViewModel>(this, ViewModelType);

			NewViewModel->Initialize(OwnerActor);

			ViewModelsRegistered.ViewModel = NewViewModel;
			ViewModelsTypeToViewModels.Add(ViewModelActorInfos, NewViewModel);

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
	
	for (const auto [ViewModel, ViewModelType] : View->GetViewModelsRegistered())
	{
		auto ViewModelActorInfos = ViewModelAndActorKey{OwnerActor->GetUniqueID(), ViewModelType};

		if(const auto ViewModelPtr = ViewModelsTypeToViewModels.Find(ViewModelActorInfos))
		{
			(*ViewModelPtr)->Destroy(OwnerActor);
			
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
