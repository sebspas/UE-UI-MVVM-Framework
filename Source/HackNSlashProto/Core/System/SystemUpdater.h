#pragma once
#include "ISystem.h"

class SystemUpdater : AActor
{
public:
	virtual void Tick(float DeltaSeconds) override;

private:	
	UPROPERTY(EditInstanceOnly)
	TArray<ISystem> Systems;

	bool IsInitialized = false;
};
