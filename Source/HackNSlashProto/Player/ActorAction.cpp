#include "ActorAction.h"

#include "HackNSlashProtoCharacter.h"
#include "HackNSlashProtoPlayerController.h"

void UActorAction::Init(UObject* OwnerActor)
{
	Owner = OwnerActor;
}

bool UActorAction::CanExecute()
{
	bool CanExecute = false;
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		CanExecute = CanMove || player->GetVelocity().Size() <= 0.f;
	}

	CanExecute &= !IsInCooldown();
	
	return CanExecute;
}

void UActorAction::Execute()
{
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		if(AHackNSlashProtoPlayerController* PlayerController = Cast<AHackNSlashProtoPlayerController>(player->GetController()))
		{
			PlayerController->RotateTowardCursor();
		}
	}
}

void UActorAction::Cancel()
{
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		player->GetMesh()->Stop();
		player->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}

void UActorAction::Tick(float DeltaSeconds)
{
	if(RemainingCooldown != 0.f)
	{
		RemainingCooldown -= DeltaSeconds;
		if(RemainingCooldown < 0.f)
		{
			RemainingCooldown = 0.f;
		}
	}
}

void UBasicAbility::Execute()
{
	UActorAction::Execute();
	
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{
		player->GetMesh()->Stop();
		player->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		player->GetMesh()->PlayAnimation(Animation, false);
	}

	RemainingCooldown = Cooldown;
}

void UBasicSpell::Execute()
{
	UActorAction::Execute();
	
	if(AHackNSlashProtoCharacter* player = Cast<AHackNSlashProtoCharacter>(Owner))
	{		
		player->GetMesh()->Stop();
		player->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		player->GetMesh()->PlayAnimation(Animation, false);

		FVector Location = player->GetActorLocation();
		FRotator Rotation = player->GetActorRotation();
		AActor* SpawnedActor = player->GetWorld()->SpawnActor(SpawnableType, &Location, &Rotation);
		if(SpawnedActor)
		{
			UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(SpawnedActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if(StaticMeshComponent)
			{
				StaticMeshComponent->SetPhysicsLinearVelocity(player->GetActorForwardVector()* Speed);
			}
		}

		RemainingCooldown = Cooldown;
	}
}
