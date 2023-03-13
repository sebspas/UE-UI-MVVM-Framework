#pragma once

#include "HackNSlashProto/Core/MVVM/ViewModel/ViewModel.h"
#include "HackNSlashProto/Game/UI/ViewModelObject/DamageNumberViewModelObject.h"

#include "DamageNumberViewModel.generated.h"

UCLASS(BlueprintType)
class UDamageNumberViewModel final : public UViewModel
{
	GENERATED_BODY()
public:
	virtual auto Initialize(AActor* Actor) -> void override;
	virtual auto OnDestroy(AActor* Actor) -> void override;
	virtual auto Update(float DeltaSeconds) -> void override;

	UFUNCTION()
	void OnHealthChanged(float Delta, float Current, float Max);

	GENERATE_UI_SETTERS(UDamageNumberViewModelObject, TArray<FDamageNumber>, DamageNumbers);
	GENERATE_UI_SETTERS_ARRAY_ELEMENT(UDamageNumberViewModelObject, FDamageNumber, DamageNumbers);
	
	GENERATE_UI_SETTERS(UDamageNumberViewModelObject, bool, IsVisibleOnScreen);
	GENERATE_UI_SETTERS(UDamageNumberViewModelObject, FVector2D, ActorScreenLocation);
	GENERATE_UI_SETTERS(UDamageNumberViewModelObject, float, DeltaTime);
};
