#pragma once

class IUpdatableSubSystem
{
public:
	virtual ~IUpdatableSubSystem() = default;
	
	virtual void Update(float DeltaSeconds) {}
};
