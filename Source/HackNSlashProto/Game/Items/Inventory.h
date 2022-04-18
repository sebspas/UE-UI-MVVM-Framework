// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Weapon.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class AEquipment;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	Weapon,
	Armor,
	Accessories
};

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class UInventorySlot : public UObject
{
	GENERATED_BODY()

public:
	void SpawnEquipment(TSubclassOf<AEquipment> EquipmentToSpawn, UMeshComponent* Mesh);

	AEquipment* GetEquipmentInSlot() const { return Equipment; }
	ESlotType GetSlotType() const { return Type; }

protected:
	UPROPERTY(EditAnywhere)
	ESlotType Type = ESlotType::Weapon;

	UPROPERTY(EditAnywhere)
	FName BoneName = "Invalid";

	UPROPERTY(VisibleAnywhere)
	AEquipment* Equipment;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACKNSLASHPROTO_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	AWeapon* GetEquippedWeapon() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UInventorySlot*> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> DefaultWeapon;
};
