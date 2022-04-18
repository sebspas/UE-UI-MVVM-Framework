// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Weapon.h"

void UInventorySlot::SpawnEquipment(TSubclassOf<AEquipment> EquipmentToSpawn, UMeshComponent* Mesh)
{
	if(Mesh)
	{
		FVector Location = Mesh->GetSocketLocation(BoneName);
		FRotator Rotation = Mesh->GetSocketRotation(BoneName);

		AActor* SpawnedActor = Mesh->GetWorld()->SpawnActor(EquipmentToSpawn, &Location, &Rotation);
		if(SpawnedActor)
		{
			const FAttachmentTransformRules TransformRule(EAttachmentRule::SnapToTarget, true);
			SpawnedActor->AttachToComponent(Mesh, TransformRule, BoneName);
			Equipment = Cast<AEquipment>(SpawnedActor);
		}
	}
}

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AWeapon* UInventory::GetEquippedWeapon() const
{
	for (auto slot : Slots)
	{
		if(slot->GetSlotType() == ESlotType::Weapon)
		{
			return Cast<AWeapon>(slot->GetEquipmentInSlot());
		}
	}

	return nullptr;
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//TODO: Remove this when we have a proper way to set default inventory
	UMeshComponent* Mesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UMeshComponent::StaticClass()));
	if(Mesh)
	{
		Slots[0]->SpawnEquipment(DefaultWeapon, Mesh);
	}
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

