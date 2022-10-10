#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "View.generated.h"

class UViewModel;

USTRUCT(BlueprintType)
struct FViewModelStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UViewModel> ViewModel;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TSubclassOf<UViewModel> ViewModelType;
};

UCLASS(BlueprintType, Blueprintable)
class UView : public UUserWidget
{
	GENERATED_BODY()

public:
	// Called by the user when creating the view, with the parent OwningActor
	UFUNCTION(BlueprintCallable)
	void InitializeView(AActor* NewOwningActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnViewInitialized();

	// Destroyed the requested ViewModels and clear the Listener
	virtual void RemoveFromParent() override;

	UFUNCTION(BlueprintCallable)
	UViewModel* GetViewModelByType(TSubclassOf<UViewModel> ViewModelType) const;
	
	AActor* GetOwningActor() const { return OwningActor; }
	
	auto GetViewModelsRegistered() -> TArray<FViewModelStruct>&;

	auto SetIsRegisteredToMvvm(bool IsRegisteredToMvvm) -> void;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<FViewModelStruct> ViewModels;

	UPROPERTY(BlueprintReadOnly)
	AActor* OwningActor;

private:
	bool IsRegisteredToMVVM = false;
};
