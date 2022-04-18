#pragma once

#include "CoreMinimal.h"
#include "HackNSlashProto/Core/MVVM/View/View.h"
#include "HackNSlashProto/Core/MVVM/ViewModelObject/ViewModelObject.h"

#include "ViewModel.generated.h"

DECLARE_DYNAMIC_DELEGATE(FViewModelPropertyChanged);

UCLASS()
class UViewModel : public UObject
{
	GENERATED_BODY()
public:
	UViewModel() {}
	virtual ~UViewModel() = default;
	
	virtual void Initalize() {}
	virtual void Update(float DeltaSeconds) {}	

	// This is used as a helped in the actual viewModel to call the templated version below
	virtual void DiffViewModelObject() {};

	UFUNCTION(BlueprintCallable)
	void RegisterOnPropertyChanged(const FName& PropertyName, const FViewModelPropertyChanged& PropertyChanged);

	UFUNCTION(BlueprintCallable)
	void UnRegisterOnPropertyChanged(const FName& PropertyName, const FViewModelPropertyChanged& PropertyChanged);

	auto OnPropertyChangedEvent(const FName& PropertyName) -> void;

	// DO Getter for both struct

protected:
	template <class ViewModel, class ViewModelObjectType>
	static void DiffViewModelObjectTemplate(ViewModel* VM, ViewModelObjectType* OldViewModelObject, ViewModelObjectType* NewViewModelObject);
	
	TSubclassOf<FViewModelObject> ViewModelObject;
	TSubclassOf<FViewModelObject> View_ViewModelObject;
	
	TMap<FName, TArray<FViewModelPropertyChanged>> RegisteredPropertyMulticast; 
};

template <class ViewModel, class ViewModelObjectType>
void UViewModel::DiffViewModelObjectTemplate(ViewModel* VM, ViewModelObjectType* OldViewModelObject, ViewModelObjectType* NewViewModelObject)
{
	TFieldIterator<FProperty> oldPropertyIt(OldViewModelObject);
	TFieldIterator<FProperty> newPropertyIt(NewViewModelObject);
	
	for (; oldPropertyIt && newPropertyIt; ++oldPropertyIt, ++newPropertyIt)
	{
		const FProperty* OldStructProp = *oldPropertyIt;
		const FProperty* NewStructProp = *oldPropertyIt;
	
		auto OldPropertyPtr = OldStructProp->ContainerPtrToValuePtr<void>((void*)OldStructProp, 0);
		auto NewPropertyPtr = NewStructProp->ContainerPtrToValuePtr<void>((void*)NewStructProp, 0);

		if(!oldPropertyIt->Identical(OldPropertyPtr, NewPropertyPtr))
		{
			// Not Identical properties, send event
			VM->OnPropertyChangedEvent(OldStructProp->GetName());
		}
	}
}
