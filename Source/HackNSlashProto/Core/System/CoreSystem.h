#pragma once

#include "CoreSystem.generated.h"

class UCoreSystem;

USTRUCT(BlueprintType)
struct FSystemInfos
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TSubclassOf<UCoreSystem> SystemType;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FName SystemName = "Default";
};

UCLASS(BlueprintType, Abstract)
class UCoreSystem : public UObject
{
public:
	GENERATED_BODY()

	UCoreSystem() = default;
	virtual ~UCoreSystem() override = default;

	virtual void Initialize(const FSystemInfos& infos);
	virtual void Update(float DeltaSeconds) {};

public:	
	UPROPERTY(BlueprintReadOnly)
	FSystemInfos SystemInfos;
};
