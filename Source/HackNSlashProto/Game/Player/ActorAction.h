// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorAction.generated.h"

class AWeapon;
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew, Abstract)
class UActorAction : public UDataAsset
{
	GENERATED_BODY()

public:
	void Init(UObject* OwnerActor);
	virtual bool CanExecute();
	virtual void Execute();
	virtual void Cancel();

	bool IsInCooldown() { return RemainingCooldown != 0.f; }
	bool IsCompleted() { return IsActionCompleted; }

	virtual void Tick(float DeltaSeconds);

	UPROPERTY(EditAnywhere)
	bool CanMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Cooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ManaCost = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RemainingCooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

protected:
	UObject* Owner = nullptr;
	bool IsActionCompleted = false;
};

UCLASS(BlueprintType)
class UBasicAbility : public UActorAction
{
	GENERATED_BODY()

public:
	UBasicAbility() {}

	virtual void Execute() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void ApplyAttack(AActor* Actor) const;

protected:
	UPROPERTY(EditAnywhere)
	float Damage = 0.f;

	UPROPERTY(EditAnywhere)
	UAnimSequence* Animation;

private:
	float AnimationLength = 0.f;
	float ElapsedAnimTime = 0.f;

	float HitTime = 0.f;
	uint8 HitCount = 0;

	// Reference on the weapon used for this attack
	AWeapon* Weapon;
	
	// We want to apply the hit as soon as we hit a new actor
	TSet<AActor*> ActorDamaged;
};

UCLASS(BlueprintType)
class UBasicSpell : public UActorAction
{
	GENERATED_BODY()

public:
	UBasicSpell() {}

	virtual void Execute() override;

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