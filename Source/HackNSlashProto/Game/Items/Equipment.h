// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HackNSlashProto/Game/Player/StatsComponent.h"

#include "Equipment.generated.h"

USTRUCT()
struct FModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Percentage = 1.f;

	UPROPERTY(EditAnywhere)
	UStat* Stat = nullptr;
};

UCLASS()
class HACKNSLASHPROTO_API AEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	AEquipment();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FStatLine> Stats;
};
