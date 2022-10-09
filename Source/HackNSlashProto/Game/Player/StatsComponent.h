// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

UCLASS()
class UStat : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FName Name;
};

USTRUCT(BlueprintType)
struct FStatLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStat* Stat = nullptr;
	
	UPROPERTY(EditAnywhere)
	float Base = 0.f;

	float Current = 0.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACKNSLASHPROTO_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FStatLine> Stats;
};
