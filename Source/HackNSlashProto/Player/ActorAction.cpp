#include "ActorAction.h"

#include "HackNSlashProtoCharacter.h"
#include "HackNSlashProtoPlayerController.h"

bool UActorAction::CanExecute(UObject* OwnerActor)
{
	if(!OwnerActor)
	{
		return false;
	}

	Owner = OwnerActor;
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		if(!CanMove)
		{
			return player->GetVelocity().Size() <= 0.f;
		}
	}
	
	return false;
}

void UActorAction::Cancel()
{
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		player->GetMesh()->Stop();
		player->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}

void UBasicAbility::Execute(UObject* OwnerActor)
{
	if(!OwnerActor)
	{
		return;
	}

	Owner = OwnerActor;
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		player->GetMesh()->Stop();
		player->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		player->GetMesh()->PlayAnimation(Animation, false);
	}
}

void UBasicSpell::Execute(UObject* OwnerActor)
{
	if(!OwnerActor)
	{
		return;
	}

	Owner = OwnerActor;
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		if(AHackNSlashProtoPlayerController* PlayerController = Cast<AHackNSlashProtoPlayerController>(player->GetController()))
		{
			PlayerController->RotateTowardCursor();
		}
		
		player->GetMesh()->Stop();
		player->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		player->GetMesh()->PlayAnimation(Animation, false);

		FVector Location = player->GetActorLocation();
		FRotator Rotator = player->GetActorRotation();
		AActor* spawnedActor = GetWorld()->SpawnActor(SpawnableType.GetDefaultObject()->GetClass(), &Location, &Rotator);
		if(spawnedActor)
		{
			UStaticMeshComponent* staticMeshComponent = Cast<UStaticMeshComponent>(spawnedActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if(staticMeshComponent)
			{
				staticMeshComponent->SetPhysicsLinearVelocity(player->GetActorForwardVector()* Speed);
			}
		}
	}
}
