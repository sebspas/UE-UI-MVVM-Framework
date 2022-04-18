#pragma once
#include "Blueprint/UserWidget.h"

#include "View.generated.h"

class UViewModel;

UCLASS(BlueprintType, Blueprintable)
class UView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Create the requested ViewModels using the MVVMSystem
	virtual bool Initialize() override;

	// Destroyed the requested ViewModels and clear the Listener
	virtual void RemoveFromParent() override;

	auto GetViewModelsRegistered() const -> const TArray<UViewModel*>&;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<UViewModel*> ViewModels;
};
