// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "ChaosInterfaceWrapperCore.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProductionTopDown/Components/InventoryComponent.h"
#include "DrawDebugHelpers.h"
#include "EnemyBase.h"
#include "kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProductionTopDown/Components/HealthComponent.h"
#include "ProductionTopDown/Components/InteractComponent.h"
#include "ProductionTopDown/Actors/Puzzle/Pushable_ActorBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
//#include "ToolContextInterfaces.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	// InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
	
	AttackRangeComponent = CreateDefaultSubobject<UBoxComponent>("Attack Range Component");
	AttackRangeComponent->SetupAttachment(CharacterMesh, TEXT("Attack Range"));
	
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(CharacterMesh, TEXT("WeaponSocket"));
	
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::StartAttackTimer);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::StopAttackTimer);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::DashEvent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::StartDrag);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::StopDrag);
	//PlayerInputComponent->BindAxis("MouseX",this, &APlayerCharacter::RotateCharacter);
}

void APlayerCharacter::TriggerDeath()
{
	Super::TriggerDeath();

	SetPlayerState(EPlayerState::Dead);
	Cast<APawn>(this)->DisableInput(CharacterController);
	//if(CharacterController)GetOwner()->DisableInput(CharacterController);
	BPTriggerDeath();
	//UE_LOG(LogTemp, Warning, TEXT("Player died"));

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	 
	//input ticks before actor. fix bug ?
	AddTickPrerequisiteActor(CharacterController);

	InventoryComponent = FindComponentByClass<UInventoryComponent>();
	InteractComponent = FindComponentByClass<UInteractComponent>();

	
	//makes the code and blueprint speed match
	ResetWalkSpeed();
	
	if(HealthComponent)HealthComponent->SetDefaultHealth(DefaultHealth);
	
	//start in moving state
	SetPlayerState(EPlayerState::Moving); 


	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();
	
	if(Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	}
	
	//Attach AttackRange to Socket
	if(AttackRangeComponent)
	{
		AttackRangeComponent->AttachToComponent(CharacterMesh,FAttachmentTransformRules::KeepRelativeTransform, TEXT("AttackRangeSocket"));
		AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::APlayerCharacter::OnComponentBeginOverlap);
		AttackRangeComponent->SetGenerateOverlapEvents(false);
	}

	CharacterController = GetWorld()->GetFirstPlayerController();
	if (CharacterController)
	{
		CharacterController->bShowMouseCursor = true;
	}
	
}
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	if(CheckForPushableActor() && GetPlayerState() == EPlayerState::Moving)
	{
		SetPlayerState(EPlayerState::Pushing);
		//walks half speed while pushing
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->GetMaxSpeed()/2;
	}
	else if(!CheckForPushableActor() && GetPlayerState() == EPlayerState::Pushing)
	{
		SetPlayerState(EPlayerState::Moving);
		ResetWalkSpeed();
	}

	if(GetActorLocation().Z < -20 && PlayerState != EPlayerState::Dead)
	{
		UGameplayStatics::ApplyDamage(this, 99999, GetInstigatorController(), this, DamageType);
	}
	
	switch (GetPlayerState())
	{
	case EPlayerState::Attacking:
		//attack state
		break;
	case EPlayerState::Dashing:
		//dashing state
		break;
	case EPlayerState::Moving:
		RotateCharacter();
		break;
	case EPlayerState::Pushing:
		RotateCharacter();
		PushObject(GetActorInFront());
		break;
	case EPlayerState::Dragging:
		DragObject(GetActorInFront());
	default:
		
		break;
	}
	GetActorInFront();
	if (bDrawAttackRangeBox)
    	{
    		DrawDebugBox(GetWorld(),AttackRangeComponent->GetComponentLocation(), AttackRangeComponent->GetScaledBoxExtent(), AttackRangeComponent->GetComponentRotation().Quaternion() ,FColor::Red,false,0.f, 0,10);
    	}
}

EPlayerState APlayerCharacter::GetPlayerState()
{
	FScopeLock Lock(&PlayerStateCriticalSection);
	return PlayerState;
}

float APlayerCharacter::GetDashTimer()
{
	return DashTimer;
}

float APlayerCharacter::GetAttackTimer()
{
	return AttackTimer;
}

void APlayerCharacter::AttackEvent()
{
	
	if(InventoryComponent->GetItemObject()
		&& InventoryComponent->GetItemObject()->IsWeapon()
		&& GetPlayerState() == EPlayerState::Moving)
	{
		Attack();
	}
	
}

bool APlayerCharacter::Attack()
{
	RotateCharToMouse();
	// returns false if there is not enough stamina
	if (!Super::Attack()) return false;
	// Attack code here
	//rotates char to cursor
	
	SetPlayerState(EPlayerState::Attacking);
	
	//makes you walk slower while attacking
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed*0.2f;
	
	LogPlayerState();
	TArray<AActor*> OverlappingActors;
	AttackRangeComponent->GetOverlappingActors(OverlappingActors);
	AItemBase* ItemBase = InventoryComponent->GetItemObject();
	if(OverlappingActors.Num() > 0 && ItemBase)
	{
		for (int i = 0; i < OverlappingActors.Num(); ++i)
		{
			UE_LOG(LogTemp, Error, TEXT("OverlappingActors %S"), *OverlappingActors[i]->GetHumanReadableName())
			if(OverlappingActors[i] != this){
			UGameplayStatics::ApplyDamage(
                            OverlappingActors[i],
                            FMath::RandRange(ItemBase->GetMinDamage(), ItemBase->GetMaxDamage()),
                            GetOwner()->GetInstigatorController(),
                            this,
                            DamageType
                            );
				
				//knockback
				const FVector PushBackVector = (OverlappingActors[i]->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
				ACharacterBase* Characterbaseptr = Cast<ACharacterBase>(OverlappingActors[i]);
				if(Characterbaseptr)Characterbaseptr->LaunchCharacter(PushBackVector*InventoryComponent->GetItemObject()->GetKnockbackAmount(), true, false);
			}
		}
	}
	
	//attack delay
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
        //code who runs after delay time
        SetPlayerState(EPlayerState::Moving);
        LogPlayerState();
		ResetWalkSpeed();
    }, AttackTimer, 0);
	return true;
}

void APlayerCharacter::DashEvent()
{
	//Dash Animation and particles
	if (GetPlayerState() == EPlayerState::Moving && bCanDash)
	{
		Dash();
	}
	
}

bool APlayerCharacter::Dash()
{
	if(GetPlayerState() != EPlayerState::Attacking)
	// returns false if there is not enough stamina
	if (!Super::Dash()) return false;
	// Dash code here
	SetPlayerState(EPlayerState::Dashing);
	bCanDash = false;

	//change collison object so it can dash trough enemies

	//GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	
	
	//particle and sounds
	if (DashSound)
		UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
	if(DashParticle)
	{
		const FVector SystemLocation = GetMesh()->GetSocketLocation("DashParticleSocket");
		const FRotator SystemRotation = GetMesh()->GetSocketRotation("DashParticleSocket");
		const FVector SystemScale = GetMesh()->GetSocketTransform("DashParticleSocket").GetScale3D();
		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            DashParticle,
            SystemLocation,
            SystemRotation,
            SystemScale,
            true,
            true,
            ENCPoolMethod::AutoRelease,
            true
            );
	}
	
	//LogPlayerState();
	// fix bug if you dash from ledge.
	
	GetCharacterMovement()->FallingLateralFriction = 8;
	//teleport player towards last direction
	FVector DashDirection = LastDirection.GetSafeNormal()*DashDistance;
	DashDirection.Z = 0;
	LaunchCharacter(DashDirection, true , false);
	
	//delay until dash is finish
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		//code who runs after delay time
		SetPlayerState(EPlayerState::Moving);
		//LogPlayerState();
		GetCharacterMovement()->FallingLateralFriction = 0;
		//GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
    }, DashTimer, 0);

	//delay between dashes.
	FTimerHandle handle2;
	GetWorld()->GetTimerManager().SetTimer(handle2, [this]()
	{
		bCanDash = true;
	}, DashDelay, 0);
	
	return true;
}

AActor* APlayerCharacter::GetActorToDamage()
{
	TArray<UPrimitiveComponent*> ComponentsArray;
	AttackRangeComponent->GetOverlappingComponents(ComponentsArray);

	for (auto Component : ComponentsArray)
	{
		if(Component->GetOwner() == this) break;
		else if(Component->IsA(UCapsuleComponent::StaticClass()))
		{
			
		}
	}
	return nullptr;
}

void APlayerCharacter::StartAttackTimer()
{
	//need weapon to attack

	if (InventoryComponent)
	{
		AItemBase* Item = InventoryComponent->GetItemObject();
		if(Item && Item->IsWeapon())
		{
			StartAttackTime = UGameplayStatics::GetTimeSeconds(GetWorld());
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed*0.2f;
		}
	}
}

void APlayerCharacter::StopAttackTimer()
{
	if (InventoryComponent)
	{
		AItemBase* Item = InventoryComponent->GetItemObject();
		if(Item && Item->IsWeapon())
		{
			StopAttackTime = UGameplayStatics::GetTimeSeconds(GetWorld());
			ResetWalkSpeed();
			CalcAttackType();
		}
	}
}

void APlayerCharacter::CalcAttackType()
{
	//if attack hold > 1 sec heavy attack
	const float AttackHoldSeconds = StopAttackTime-StartAttackTime;
	if(PlayerState != EPlayerState::Moving) return;
	if(AttackHoldSeconds < 1)
	{
		if(!Super::Attack()) return;
		RotateCharToMouse();
		LightAttack();
		
	}
	else
	{
		if(!Super::Attack()) return;
		RotateCharToMouse();
		HeavyAttack();
		
	}

}

void APlayerCharacter::LightAttack()
{
	//light attack particle
	SetPlayerState(EPlayerState::Attacking);

	const FVector BoxSize{60,80,50};
	AttackRangeComponent->SetBoxExtent(BoxSize,true);
	//SetBoxSize
	
	bHeavyAttack = false;
	
	if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(true);
	
	if(LightAttackParticle)
	{
		const FVector SystemLocation = GetMesh()->GetSocketLocation("AttackParticleSocket");
		const FRotator SystemRotation = GetMesh()->GetSocketRotation("AttackParticleSocket");
		const FVector SystemScale = GetMesh()->GetSocketTransform("AttackParticleSocket").GetScale3D();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        LightAttackParticle,
        SystemLocation,
        SystemRotation,
        SystemScale,
        true,
        true,
        ENCPoolMethod::AutoRelease,
        true
        );
	}
	
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
        //code who runs after delay time
        SetPlayerState(EPlayerState::Moving);
		if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(false);
    }, AttackTimer, 0);
}

void APlayerCharacter::HeavyAttack()
{

	SetPlayerState(EPlayerState::Attacking);

	const FVector BoxSize{120,80,50};
	AttackRangeComponent->SetBoxExtent(BoxSize,true);
	//SetBoxRange
	
	bHeavyAttack = true;
	
	if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(true);
	
	if(HeavyAttackParticle)
	{
		const FVector SystemLocation = GetMesh()->GetSocketLocation("AttackParticleSocket");
		const FRotator SystemRotation = GetMesh()->GetSocketRotation("AttackParticleSocket");
		const FVector SystemScale = GetMesh()->GetSocketTransform("AttackParticleSocket").GetScale3D();
		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            HeavyAttackParticle,
            SystemLocation,
            SystemRotation,
            SystemScale,
            true,
            true,
            ENCPoolMethod::AutoRelease,
            true
            );
	}
	
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
        //code who runs after delay time
        SetPlayerState(EPlayerState::Moving);
		if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(false);
    }, AttackTimer, 0);
	
}

float APlayerCharacter::GetAttackDamage()
{
	float Damage{0};
	
	if (bHeavyAttack && InventoryComponent->GetItemObject()->IsWeapon())
	{
		//heavy attack damage
		Damage = FMath::RandRange(
        InventoryComponent->GetItemObject()->GetMinDamage(),
        InventoryComponent->GetItemObject()->GetMaxDamage()
        );
		Damage *= 2;
	}
	else if(InventoryComponent->GetItemObject()->IsWeapon())
	{
		//light attack damage, make this fancy some time
		Damage = FMath::RandRange(
        InventoryComponent->GetItemObject()->GetMinDamage(),
        InventoryComponent->GetItemObject()->GetMaxDamage()
        );
	}
	return Damage;
}


void APlayerCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void APlayerCharacter::RotateCharacter()
{
	
	float VLen = GetVelocity().Size();
	FRotator MeshRotation = GetVelocity().Rotation();
	MeshRotation.Yaw -= 90; //rotates the char så den blir rett vei
	MeshRotation.Pitch = 0;
	
	if (CharacterMesh && VLen != 0)
	{
		LastDirection = GetVelocity();
		LastRotation = MeshRotation;
		CharacterMesh->SetWorldRotation(MeshRotation);
	}
}

void APlayerCharacter::RotateCharToMouse()
{
	FHitResult HitResult;
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	CharacterController->GetHitResultUnderCursorByChannel(TraceChannel,true, HitResult);
	
	const FVector DirectionVector =( HitResult.Location - GetActorLocation()).GetSafeNormal2D();
	FRotator MeshRotation = DirectionVector.Rotation();
	MeshRotation.Yaw -= 90;
	MeshRotation.Pitch = 0;
	MeshRotation.Roll = 0;
	
	CharacterMesh->SetWorldRotation(MeshRotation);

}

void APlayerCharacter::EquipWeaponFromInv(UStaticMesh* EquipWeapon)
{
	//UE_LOG(LogTemp, Warning, TEXT("Trying to equip Weapon"));
	Weapon->SetStaticMesh(EquipWeapon);
	//EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
}

void APlayerCharacter::StartDrag()
{
	if(AbleToDrag()) // able to grab
	{
		SetPlayerState(EPlayerState::Dragging);
	}
	
}

void APlayerCharacter::StopDrag()
{
	if(GetPlayerState() == EPlayerState::Dragging)
		SetPlayerState(EPlayerState::Moving);

	ResetWalkSpeed();
}

bool APlayerCharacter::CheckForPushableActor()
{
	if(GetActorInFront()) return true;
	return false;
}

bool APlayerCharacter::AbleToDrag()
{
	if(CheckForPushableActor())return true;
	return false;
}

APushable_ActorBase* APlayerCharacter::GetPushableActor()
{
	TArray<AActor*> OverlappingActors;
	AttackRangeComponent->GetOverlappingActors(OverlappingActors);
	for (int i = 0; i < OverlappingActors.Num(); ++i)
	{
		if(OverlappingActors[i]->IsA(APushable_ActorBase::StaticClass()))
		{
			return Cast<APushable_ActorBase>(OverlappingActors[i]);
		}
	}
	return nullptr;
}

void APlayerCharacter::PushObject(APushable_ActorBase* PushableActor)
{
	//locks movement to x or y axis
	FVector Velocity = GetVelocity();
	//UE_LOG(LogTemp, Error, TEXT("Velocity before: %s"), *Velocity.ToString())
	if (abs(Velocity.X) > abs(Velocity.Y))
	{
		Velocity.Y = 0;
	}
	else Velocity.X = 0;
	
	//UE_LOG(LogTemp, Error, TEXT("Velocity After: %s"), *Velocity.ToString())
	if(PushableActor && bCanPush)
	{
		FVector PushDirection = PushableActor->GetActorLocation();
		PushDirection += Velocity.GetSafeNormal2D()*PushDistance;
		PushDirection.Z = PushableActor->GetActorLocation().Z;
		//Calculate new position by velocity from char;
		PushableActor->SetActorLocation(PushDirection, true);
	}

}

//dont know if i need this one
void APlayerCharacter::DragObject(APushable_ActorBase* PushableActor)
{
	if(PushableActor)
	{
		FVector PushDirection = PushableActor->GetActorLocation();
		PushDirection += LastDirection.GetSafeNormal2D()*PushDistance;
		PushDirection.Z = PushableActor->GetActorLocation().Z;
		//Calculate new position by velocity from char;
		PushableActor->SetActorLocation(PushDirection, true);
	}
}

APushable_ActorBase* APlayerCharacter::GetActorInFront()
{
	APushable_ActorBase* ActorInFront = nullptr;
	FHitResult Hit;
	FVector TraceEnd = GetActorLocation() + LastDirection.GetSafeNormal() * PushRange;
	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_WorldDynamic;
	FCollisionQueryParams CollisionParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByObjectType(Hit, GetActorLocation(), TraceEnd, CollisionChannel, CollisionParams);

	if((Hit.Location - GetActorLocation()).Size() < PushRange/2) bCanPush = true;
	else bCanPush = false;
	
	//set to false to stop drawing debug lines
	bool BDrawDebugLine{false};
	if(BDrawDebugLine){
		if(Hit.IsValidBlockingHit())DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				Hit.Location,
				FColor::Green,
				false,
				0.f,
				0.f,
				5.f
				);
		 else DrawDebugLine(
	            GetWorld(),
	            GetActorLocation(),
	            TraceEnd,
	            FColor::Red,
	            false,
	            0.f,
	            0.f,
	            5.f
	            );
	}
		ActorInFront = Cast<APushable_ActorBase>(Hit.Actor);

	return ActorInFront;
}

void APlayerCharacter::OnInventoryChange()
{
	if (InventoryComponent)
	{
		AItemBase* CurrentItemBase = InventoryComponent->GetItemObject();
		UStaticMesh* CurrentItemMesh = nullptr;
		if(CurrentItemBase)CurrentItemMesh = CurrentItemBase->FindComponentByClass<UStaticMeshComponent>()->GetStaticMesh();;
		//if(CurrentItemBase)GetWorld()->SpawnActor<AItemBase>(CurrentItemBase, GetActorLocation(), GetActorRotation());
		if(CurrentItemBase)EquipWeaponFromInv(CurrentItemMesh);
		else EquipWeaponFromInv(nullptr); // equip nothing ? 
		if(!CurrentItemMesh) UE_LOG(LogTemp, Error, TEXT("Item Mesh not found"));
		if(!CurrentItemBase) UE_LOG(LogTemp, Error, TEXT("Item Base not found"))
	}
}


void APlayerCharacter::LogPlayerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Player state is : %i"), GetPlayerState());
}

void APlayerCharacter::SetPlayerState(EPlayerState inpPlayerState)
{
	FScopeLock Lock(&PlayerStateCriticalSection);
	PlayerState = inpPlayerState;
}

void APlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != this)
	{
		if(OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherComp->GetOwner()->IsA(ACharacterBase::StaticClass())
			|| !OtherActor->IsA(ACharacterBase::StaticClass()))
		{
			UGameplayStatics::ApplyDamage(
                    OtherComp->GetOwner(),
                    GetAttackDamage(),
                    GetInstigatorController(),
                    this,
                    DamageType
                    );

			if(bHeavyAttack)
			{
				FVector PushBackVector = (OtherComp->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
				PushBackVector *=2;				
				ACharacterBase* Characterbaseptr = Cast<ACharacterBase>(OtherComp->GetOwner());
				if(Characterbaseptr)Characterbaseptr->LaunchCharacter(PushBackVector*InventoryComponent->GetItemObject()->GetKnockbackAmount(), true, false);
			}
			else
			{
				const FVector PushBackVector = (OtherComp->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
				ACharacterBase* Characterbaseptr = Cast<ACharacterBase>(OtherComp->GetOwner());
				if(Characterbaseptr)Characterbaseptr->LaunchCharacter(PushBackVector*InventoryComponent->GetItemObject()->GetKnockbackAmount(), true, false);
			}
		}
	}
}

void APlayerCharacter::ResetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}
