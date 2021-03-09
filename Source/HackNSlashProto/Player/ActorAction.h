// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorAction.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class UActorAction : public UObject
{
	GENERATED_BODY()

public:
	virtual bool CanExecute(UObject* OwnerActor);
	virtual void Execute(UObject* OwnerActor) {}
	virtual void Cancel();

	UPROPERTY(EditAnywhere)
	bool CanMove = false;

protected:
	UObject* Owner = nullptr;
};

UCLASS(BlueprintType)
class UBasicAbility : public UActorAction
{
	GENERATED_BODY()

public:
	UBasicAbility() {}

	virtual void Execute(UObject* OwnerActor) override;

protected:
	UPROPERTY(EditAnywhere)
	float Damage = 0.f;

	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation; 
};

UCLASS(BlueprintType)
class UBasicSpell : public UActorAction
{
	GENERATED_BODY()

public:
	UBasicSpell() {}

	virtual void Execute(UObject* OwnerActor) override;

protected:
	UPROPERTY(EditAnywhere)
	float Damage = 0.f;

	UPROPERTY(EditAnywhere)
	bool SpawnAtCursorLocation = false;
	
	UPROPERTY(EditAnywhere)
	float Speed = 0.f;

	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SpawnableType;
};