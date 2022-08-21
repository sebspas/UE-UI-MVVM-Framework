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

	auto ProcessChanges() -> void;

	UFUNCTION(BlueprintCallable)
	void RegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChanged);

	UFUNCTION(BlueprintCallable)
	void UnRegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChanged);

	auto OnPropertyChangedEvent(const FName& PropertyName) -> void;

	UFUNCTION(BlueprintCallable)
	UViewModelObject* GetViewModelObject() const { return View_ViewModelObject; }

protected:
	void QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const FName& PropertyChange);
	void QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const TArray<FName>& PropertiesChange);

	UPROPERTY()
	UViewModelObject* ViewModelObject;

	UPROPERTY()
	UViewModelObject* View_ViewModelObject;
	
private:
	TMap<FName, TArray<FViewModelPropertyChanged>> RegisteredPropertyMulticast;

	struct SPropertiesChange
	{
		TArray<FName> PropertiesChanged;
		std::function<void(UViewModelObject*)> ChangeLambda;
	};
	
	TQueue<SPropertiesChange> ViewModelToViewQueue;
};
