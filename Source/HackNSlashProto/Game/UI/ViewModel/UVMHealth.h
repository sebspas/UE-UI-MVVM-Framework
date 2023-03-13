#pragma once

#include "MVVMViewModelBase.h"

#include "UVMHealth.generated.h"

#define LOCTEXT_NAMESPACE "UINamespace"

UCLASS(BlueprintType)
class UVMHealth : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 MaxHealth;

	void SetCurrentHealth(int32 newCurrentHealth);

	void SetMaxHealth(int32 newMaxHealth);
	
private:
	int32 GetCurrentHealth() const
	{
		return CurrentHealth;
	}

	int32 GetMaxHealth() const
	{
		return MaxHealth;
	}

public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercent() const
	{
		if (MaxHealth != 0)
		{
			return static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
		}
		
		return  0;
	}

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetHealthText() const
	{
		return  FText::Format(LOCTEXT("HealthText", "{0}/{1}"), FText::AsNumber(CurrentHealth), FText::AsNumber(MaxHealth));
	}
};
#undef LOCTEXT_NAMESPACE