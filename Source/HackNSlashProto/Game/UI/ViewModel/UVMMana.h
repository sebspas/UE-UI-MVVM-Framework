#pragma once

#include "MVVMViewModelBase.h"
#include "UVMMana.generated.h"

#define LOCTEXT_NAMESPACE "UINamespace"

UCLASS(BlueprintType)
class UVMMana : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentMana;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 MaxMana;

	void SetCurrentMana(int32 newCurrentMana);

	void SetMaxMana(int32 newMaxMana);
	
private:
	int32 GetCurrentMana() const
	{
		return CurrentMana;
	}

	int32 GetMaxMana() const
	{
		return MaxMana;
	}

public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetManaAsPercentage() const
	{
		if (MaxMana != 0)
		{
			return static_cast<float>(CurrentMana) / static_cast<float>(MaxMana);
		}
		
		return  0;
	}

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetManaText() const
	{
		return  FText::Format(LOCTEXT("ManaText", "{0}/{1}"), FText::AsNumber(CurrentMana), FText::AsNumber(MaxMana));
	}
};
#undef LOCTEXT_NAMESPACE