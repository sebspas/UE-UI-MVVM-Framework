#pragma once

class ISystem
{
public:
	virtual ~ISystem() = default;

	virtual void Initialize() = 0;
	virtual void Update(float DeltaTime) = 0;

private:	
	UPROPERTY(EditInstanceOnly)
	FName Name = "Empty";
};
