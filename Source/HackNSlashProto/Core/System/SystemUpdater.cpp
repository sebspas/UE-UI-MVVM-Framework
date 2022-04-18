#include "SystemUpdater.h"

void SystemUpdater::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);

	if(!IsInitialized)
	{
		for (auto& system : Systems)
		{
			system.Initialize();
		}

		IsInitialized = true;
	}

	// This will need to be more elaborate and 
	for (auto& system : Systems)
	{
		system.Update(DeltaSeconds);
	}
}
