// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Equipment.h"
#include "Weapon.generated.h"

UCLASS()
class HACKNSLASHPROTO_API AWeapon : public AEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TArray<FModifier> Modifiers;
};
