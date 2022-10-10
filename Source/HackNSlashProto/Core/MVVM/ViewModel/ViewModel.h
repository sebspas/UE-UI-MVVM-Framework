﻿#pragma once

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
	
	virtual void Initialize(AActor* OwnerActor) {}
	virtual void Destroy(AActor* OwnerActor) {}
	virtual void Update(float DeltaSeconds) {}

	auto ProcessChanges() -> void;

	UFUNCTION(BlueprintCallable)
	void RegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChangedDelegate);

	UFUNCTION(BlueprintCallable)
	void UnRegisterOnPropertyChanged(FName PropertyName, const FViewModelPropertyChanged& PropertyChangedDelegate);

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
