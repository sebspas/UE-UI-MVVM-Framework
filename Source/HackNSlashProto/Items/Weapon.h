// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Equipment.h"
#include "Weapon.generated.h"

UCLASS()
class HACKNSLASHPROTO_API AWeapon : public AEquipment
{
	GENERATED_BODY()

public:
	TSet<AActor*> GetActorsHit() const { return ActorsHit; }

	void ClearActorsHit();
	
	UPROPERTY(EditAnywhere)
	TArray<FModifier> Modifiers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<AActor*> ActorsHit;
};
