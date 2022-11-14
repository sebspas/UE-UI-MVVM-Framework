#pragma once

#include "CoreMinimal.h"
#include "../View/View.h"
#include "../ViewModelObject/ViewModelObject.h"

#include "ViewModel.generated.h"

#define GENERATE_UI_SETTERS(VMType, MemberType, VarName) \
	void Set_##VarName(const MemberType& NewValue) \
	{ \
		QueueVMObjectChange([NewValue](UViewModelObject* ViewModelModelObject) \
		{ \
			auto VMO = dynamic_cast<VMType*>(ViewModelModelObject); \
			VMO->##VarName = NewValue; \
		}, {FName(#VarName)}); \
	}

#define GENERATE_UI_SETTERS_ARRAY_ELEMENT(VMType, MemberType, VarName) \
	void Set_##VarName(uint8 Index, MemberType NewValue) \
	{ \
		QueueVMObjectChange([NewValue, Index](UViewModelObject* ViewModelModelObject) \
		{ \
			auto VMO = dynamic_cast<VMType*>(ViewModelModelObject); \
			VMO->##VarName[Index] = NewValue; \
		}, {FName(#VarName)}); \
	}

DECLARE_DYNAMIC_DELEGATE(FViewModelPropertyChanged);

UCLASS(BlueprintType, Abstract)
class UViewModel : public UObject
{
	GENERATED_BODY()
public:
	UViewModel() {}
	virtual ~UViewModel() override = default;
	
	virtual void Initialize(AActor* Actor);
	void Destroy(AActor* Actor);
	
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
	
	void QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const FName& PropertyChange);
	void QueueVMObjectChange(std::function<void(UViewModelObject*)> LambdaChange, const TArray<FName>& PropertiesChange);
	
protected:
	virtual void OnDestroy(AActor* Actor) {}
	
	// The data to work with in the ViewModel might differ from the View until updated
	UPROPERTY()
	UViewModelObject* ViewModelObject;

	// Copy used by the view 
	UPROPERTY()
	UViewModelObject* View_ViewModelObject;

	UPROPERTY()
	AActor* OwnerActor = nullptr;

	TMap<FName, TArray<FViewModelPropertyChanged>> RegisteredPropertyMulticast;
	
private:
	struct FPropertiesChange
	{
		TArray<FName> PropertiesChanged;
		std::function<void(UViewModelObject*)> ChangeLambda;
	};
	
	TQueue<FPropertiesChange> ViewModelToViewQueue;
};
