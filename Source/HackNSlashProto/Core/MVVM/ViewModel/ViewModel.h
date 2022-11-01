#pragma once

#include "CoreMinimal.h"
#include "../View/View.h"
#include "../ViewModelObject/ViewModelObject.h"

#include "ViewModel.generated.h"

DECLARE_DYNAMIC_DELEGATE(FViewModelPropertyChanged);

UCLASS(BlueprintType, Abstract)
class UViewModel : public UObject
{
	GENERATED_BODY()
public:
	UViewModel() {}
	virtual ~UViewModel() override = default;
	
	virtual void Initialize(AActor* Actor);
	virtual void Destroy(AActor* Actor) {}
	virtual void Update(float DeltaSeconds) {}

	auto ProcessChanges() -> void;

	UFUNCTION(BlueprintCallable)
	void RegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChangedDelegate);

	UFUNCTION(BlueprintCallable)
	void UnRegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChangedDelegate);

	auto OnPropertyChangedEvent(const FName& PropertyName) -> void;

	UFUNCTION(BlueprintCallable)
	UViewModelObject* GetViewModelObject() const { return View_ViewModelObject; }

	/**
	 * @brief To Use only for debug, the only ViewModelObject that should be edited is the one of the view
	 * @return UViewModelObject* ViewModelObject
	 */
	UViewModelObject* GetInternalViewModelObject() const { return ViewModelObject; }

protected:
	void QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const FName& PropertyChange);
	void QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const TArray<FName>& PropertiesChange);

	// The data to work with in the ViewModel might differ from the View until updated
	UPROPERTY()
	UViewModelObject* ViewModelObject;

	// Copy used by the view 
	UPROPERTY()
	UViewModelObject* View_ViewModelObject;

	AActor* OwnerActor = nullptr;
	
private:
	TMap<FName, TArray<FViewModelPropertyChanged>> RegisteredPropertyMulticast;

	struct SPropertiesChange
	{
		TArray<FName> PropertiesChanged;
		std::function<void(UViewModelObject*)> ChangeLambda;
	};
	
	TQueue<SPropertiesChange> ViewModelToViewQueue;
};
