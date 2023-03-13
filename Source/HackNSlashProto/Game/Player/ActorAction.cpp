#include "ActorAction.h"

#include "HackNSlashProtoCharacter.h"
#include "HackNSlashProto/HackNSlashProto.h"
#include "HackNSlashProtoPlayerController.h"
#include "HackNSlashProto/Core/System/ErrorDefine.h"
#include "HackNSlashProto/Game/Component/Health.h"
#include "HackNSlashProto/Game/Component/Mana.h"
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

		if(UMana* Mana = Cast<UMana>(player->GetComponentByClass(UMana::StaticClass())))
		{
			CanExecute &= Mana->HasEnoughMana(ManaCost);
		}
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

		if(UMana* Mana = Cast<UMana>(player->GetComponentByClass(UMana::StaticClass())))
		{
			Mana->ApplyManaChange(-ManaCost);
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

		AnimationLength = Animation->GetNumberOfSampledKeys() / Animation->GetSamplingFrameRate().AsDecimal();

		FAnimNotifyContext NotifyContext = FAnimNotifyContext();
		Animation->GetAnimNotifies(0.f, AnimationLength, NotifyContext);

		for(auto NotifyEvent : NotifyContext.ActiveNotifies)
		{
			if(NotifyEvent.GetNotify()->GetNotifyEventName() == FName("AnimNotify_Hit"))
			{
				HitTime = NotifyEvent.GetNotify()->GetTriggerTime();
				CORE_LOGM(LogHackNSlashProto, "Notify found at time %f for Anim: %s", *Animation->GetName());
			}
		}

		HitCount = 1;

		ensureMsgf(HitTime != 0.f, TEXT("Couldn't find a valid HitTime for Anim %s"), *Animation->GetName());
	}

	RemainingCooldown = Cooldown;
}

void UBasicAbility::Tick(float DeltaSeconds)
{
	UActorAction::Tick(DeltaSeconds);
	
	if(AnimationLength != 0)
	{
		ElapsedAnimTime += DeltaSeconds;
		
		if(ElapsedAnimTime >= AnimationLength)
		{
			AnimationLength = 0.f;
			ElapsedAnimTime = 0.f;
		}

		if(ElapsedAnimTime >= HitTime && HitCount > 0)
		{
			for (auto Actor : Weapon->GetActorsHit())
			{
				if(Actor == Owner)
				{ continue; }
				
				if(!ActorDamaged.Contains(Actor))
				{
					ActorDamaged.Add(Actor);
					ApplyAttack(Actor);
				}
			}
			
			Weapon->GetActorsHit().Reset();
			HitCount = 0;
		}
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
