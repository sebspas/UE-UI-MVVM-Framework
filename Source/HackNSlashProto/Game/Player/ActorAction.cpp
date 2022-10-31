#include "ActorAction.h"

#include "HackNSlashProtoCharacter.h"
#include "HackNSlashProtoPlayerController.h"
#include "HackNSlashProto/Game/Component/Health.h"
#include "HackNSlashProto/Game/Items/Inventory.h"
#include "HackNSlashProto/Game/Items/Weapon.h"

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
		UInventory* Inventory = Cast<UInventory>(player->GetComponentByClass(UInventory::StaticClass()));
		if(Inventory)
		{
			Weapon = Inventory->GetEquippedWeapon();

			if(Weapon)
			{
				Weapon->ClearActorsHit();
			}
		}

		ActorDamaged.Reset();
		
		player->GetMesh()->Stop();
		player->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		player->GetMesh()->PlayAnimation(Animation, false);

		AnimationLength = Animation->GetNumberOfSampledKeys() / Animation->GetFrameRate();
	}

	RemainingCooldown = Cooldown;
}

void UBasicAbility::Tick(float DeltaSeconds)
{
	UActorAction::Tick(DeltaSeconds);
	
	if(AnimationLength != 0)
	{
		AnimationLength -= DeltaSeconds;
		
		if(AnimationLength < 0.f)
		{
			AnimationLength = 0.f;
		}
		else
		{
			for (auto Actor : Weapon->GetActorsHit())
			{
				if(!ActorDamaged.Contains(Actor))
				{
					ApplyAttack(Actor);
					ActorDamaged.Add(Actor);
				}
			}
		}
		
		Weapon->GetActorsHit().Reset();
	}
}

void UBasicAbility::ApplyAttack(AActor* Actor) const
{
	UHealth* ActorHealth = Cast<UHealth>(Actor->GetComponentByClass(UHealth::StaticClass()));
	if(ActorHealth)
	{
		ActorHealth->ApplyHealthChange(-Damage);
	}
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
