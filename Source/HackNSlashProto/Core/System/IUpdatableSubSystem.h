#pragma once

class IUpdatableSubSystem
{
public:
	virtual ~IUpdatableSubSystem() = default;

	auto GetSystemName() const -> FName { return Name; }
	
	virtual void Update(float DeltaSeconds) {}

#if WITH_IMGUI
	virtual void UpdateImGuiSystemWindow(bool& IsWindowOpen) {}
#endif

protected:
	FName Name = "Empty";
};
