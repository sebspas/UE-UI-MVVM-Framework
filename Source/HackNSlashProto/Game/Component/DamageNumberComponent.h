#pragma once

#include "CoreMinimal.h"
#include "../UI/ViewModel/UVMDamageNumbers.h"
#include "Components/ActorComponent.h"

#include "DamageNumberComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HACKNSLASHPROTO_API UDamageNumberComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageNumberComponent();

	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnHealthChanged(float Delta, float Current, float Max);

	UPROPERTY(BlueprintReadWrite)
	UVMDamageNumbers* DamageNumbers;
};
