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
	// Create the requested ViewModels using the MVVMSystem
	virtual bool Initialize() override;

	// Destroyed the requested ViewModels and clear the Listener
	virtual void RemoveFromParent() override;

	UFUNCTION(BlueprintCallable)
	UViewModel* GetViewModelByType(TSubclassOf<UViewModel> ViewModelType) const;
	
	auto GetViewModelsRegistered() -> TArray<FViewModelStruct>&;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<FViewModelStruct> ViewModels;
};
