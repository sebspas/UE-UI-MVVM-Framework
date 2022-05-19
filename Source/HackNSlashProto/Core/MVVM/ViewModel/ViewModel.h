#pragma once

#include "CoreMinimal.h"
#include "HackNSlashProto/Core/MVVM/View/View.h"
#include "HackNSlashProto/Core/MVVM/ViewModelObject/ViewModelObject.h"

#include "ViewModel.generated.h"

DECLARE_DYNAMIC_DELEGATE(FViewModelPropertyChanged);

UCLASS(BlueprintType, Abstract)
class UViewModel : public UObject
{
	GENERATED_BODY()
public:
	UViewModel() {}
	virtual ~UViewModel() override = default;
	
	virtual void Initalize() {}
	virtual void Update(float DeltaSeconds) {}	

	// This is used as a helped in the actual viewModel to call the templated version below
	virtual void DiffViewModelObject() PURE_VIRTUAL(UViewModel::DiffViewModelObject,);

	UFUNCTION(BlueprintCallable)
	void RegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChanged);

	UFUNCTION(BlueprintCallable)
	void UnRegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChanged);

	auto OnPropertyChangedEvent(const FName& PropertyName) -> void;

	UFUNCTION(BlueprintCallable)
	UViewModelObject* GetViewModelObject() const { return View_ViewModelObject; }

protected:
	template <class ViewModel, class ViewModelObjectType>
	static void DiffViewModelObjectTemplate(ViewModel* VM, ViewModelObjectType* OldViewModelObject, ViewModelObjectType* NewViewModelObject);

	UPROPERTY()
	UViewModelObject* ViewModelObject;

	UPROPERTY()
	UViewModelObject* View_ViewModelObject;
	
	TMap<FName, TArray<FViewModelPropertyChanged>> RegisteredPropertyMulticast;
};

template <class ViewModel, class ViewModelObjectType>
void UViewModel::DiffViewModelObjectTemplate(ViewModel* VM, ViewModelObjectType* OldViewModelObject, ViewModelObjectType* NewViewModelObject)
{
	TFieldIterator<FProperty> oldPropertyIt(OldViewModelObject->GetClass());
	TFieldIterator<FProperty> newPropertyIt(NewViewModelObject->GetClass());
	
	for (; oldPropertyIt && newPropertyIt; ++oldPropertyIt, ++newPropertyIt)
	{
		const FProperty* OldStructProp = *oldPropertyIt;
		const FProperty* NewStructProp = *newPropertyIt;
	
		auto OldPropertyPtr = OldStructProp->ContainerPtrToValuePtr<void>(OldViewModelObject);
		auto NewPropertyPtr = NewStructProp->ContainerPtrToValuePtr<void>(NewViewModelObject);

		if(!oldPropertyIt->Identical(OldPropertyPtr, NewPropertyPtr))
		{
			//TODO not sure the compare is properly working
			//TODO the view_object is not actually updated anywhere so even if the compare work it's always the old value
			// Not Identical properties, send event
			FName PropertyFName(OldStructProp->GetName());
			VM->OnPropertyChangedEvent(PropertyFName);
		}
	}
}
