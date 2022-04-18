// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "ActorAction.h"
#include "GameFramework/Character.h"

#include "HackNSlashProtoCharacter.generated.h"

class UMaterial;

UCLASS(Blueprintable)
class AHackNSlashProtoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHackNSlashProtoCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	void ExecuteAction(uint32 index);
	void CancelCurrentAction();

	UActorAction* GetCurrentAction();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Camera)
	UMaterial* ClickDecal;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UDecalComponent* CursorToWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	class UInventory* Inventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	TArray<UActorAction*> Actions;

private:
	int CurrentActionIndex = -1;
};

