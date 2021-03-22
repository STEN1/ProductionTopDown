// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProductionTopDown/Components/InventoryComponent.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "ProductionTopDown/Components/HealthComponent.h"
#include "ProductionTopDown/Components/InteractComponent.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
	
	AttackRangeComponent = CreateDefaultSubobject<UBoxComponent>("Attack Range Component");
	AttackRangeComponent->SetupAttachment(CharacterMesh, TEXT("Attack Range"));
	
	//Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	//Weapon->SetupAttachment(CharacterMesh, TEXT("WeaponSocket"));
	
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackEvent);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::DashEvent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("MouseX",this, &APlayerCharacter::RotateCharacter);
}

void APlayerCharacter::TriggerDeath()
{
	Super::TriggerDeath();
	UE_LOG(LogTemp, Warning, TEXT("Player died"));

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//makes the code and blueprint speed match
	ResetWalkSpeed();
	
	if(HealthComponent)HealthComponent->SetDefaultHealth(DefaultHealth);
	
	//start in moving state
	PlayerState = EPlayerState::Moving;
	LogPlayerState();

	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();

	//Attach AttackRange to Socket
	if(AttackRangeComponent)AttackRangeComponent->AttachToComponent(CharacterMesh,FAttachmentTransformRules::KeepRelativeTransform, TEXT("AttackRangeSocket"));
	
	//testing purposes
	if(Weapon)
	{
		//UE_LOG(LogTemp, Warning, TEXT("weapon and attackrange found"))
		//Weapon->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnWeaponHit);
		//AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlap);
		
		//Weapon->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlap);
		//EquipWeaponFromInv(Weapon);
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
	
	if(PlayerState != EPlayerState::Dashing)RotateCharacter();

	switch (PlayerState)
	{
	case EPlayerState::Attacking:
		//attack state
		break;
	case EPlayerState::Dashing:
		//dashing state
		break;
	case EPlayerState::Moving:
		//move state
		break;

	default:
		
		break;
	}
	
	if (bDrawAttackRangeBox)
    	{
    		DrawDebugBox(GetWorld(),AttackRangeComponent->GetComponentLocation(), AttackRangeComponent->GetScaledBoxExtent(), AttackRangeComponent->GetComponentRotation().Quaternion() ,FColor::Red,false,0.f, 0,10);
    	}
}

EPlayerState APlayerCharacter::GetPlayerState()
{
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
	
	if(InventoryComponent->GetItemObject()!= nullptr
		&& InventoryComponent->GetItemObject()->IsWeapon()
		&& PlayerState == EPlayerState::Moving)
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
	
	PlayerState = EPlayerState::Attacking;
	
	
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
			if(OverlappingActors[i] != this)
			UGameplayStatics::ApplyDamage(
                            OverlappingActors[i],
                            FMath::RandRange(ItemBase->GetMinDamage(),
                            ItemBase->GetMaxDamage()),
                            GetOwner()->GetInstigatorController(),
                            this,
                            DamageType
                            );
		}
	}
	
	//attack delay
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
        //code who runs after delay time
        PlayerState = EPlayerState::Moving;
        LogPlayerState();
		ResetWalkSpeed();
    }, AttackTimer, 0);
	return true;
}

void APlayerCharacter::DashEvent()
{
	//Dash Animation and particles
	if (PlayerState == EPlayerState::Moving && bCanDash)
	{
		Dash();
	}
	
}

bool APlayerCharacter::Dash()
{
	if(PlayerState != EPlayerState::Attacking)
	// returns false if there is not enough stamina
	if (!Super::Dash()) return false;
	// Dash code here
	PlayerState = EPlayerState::Dashing;
	bCanDash = false;
	//particle and sounds
	if (DashSound)
		UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
	if (DashParticle)
		UGameplayStatics::SpawnEmitterAtLocation(this, DashParticle, GetActorLocation());
	SpawnDashParticle();
	
	LogPlayerState();
	// fix bug if you dash from ledge.
	GetCharacterMovement()->FallingLateralFriction = 8;
	//teleport player towards last direction
	FVector DashDirection  = LastDirection.GetSafeNormal()*DashDistance;
	DashDirection.Z = 0;
	LaunchCharacter(DashDirection, true , false);
	
	//delay until dash is finish
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		//code who runs after delay time
		PlayerState = EPlayerState::Moving;
		LogPlayerState();
		GetCharacterMovement()->FallingLateralFriction = 0;
    }, DashTimer, 0);

	//delay between dashes.
	FTimerHandle handle2;
	GetWorld()->GetTimerManager().SetTimer(handle2, [this]()
	{
		bCanDash = true;
	}, DashDelay, 0);
	
	return true;
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
	//SetActorRotation(ActorRotation);
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

void APlayerCharacter::EquipWeaponFromInv(UStaticMeshComponent* EquipWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to equip Weapon"));
	EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
}

void APlayerCharacter::OnInventoryChange()
{
	AItemBase* CurrentItemBase = InventoryComponent->GetItemObject();
	UStaticMeshComponent* CurrentItemMesh = Cast<UStaticMeshComponent>(CurrentItemBase);
	//if(CurrentItemBase)GetWorld()->SpawnActor<AItemBase>(CurrentItemBase, GetActorLocation(), GetActorRotation());
	if(CurrentItemMesh)EquipWeaponFromInv(CurrentItemMesh);
	if(!CurrentItemMesh) UE_LOG(LogTemp, Error, TEXT("Item Mesh not found"));
	if(!CurrentItemBase) UE_LOG(LogTemp, Error, TEXT("Item Base not found"))
}

void APlayerCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == this) return;
	UE_LOG(LogTemp, Error, TEXT("Other Actor hit : %s"), *OtherActor->GetName())
	AItemBase* ItemBase = InventoryComponent->GetItemObject();
	if(ItemBase && OtherActor != this)UGameplayStatics::ApplyDamage(
							OtherActor, FMath::RandRange(ItemBase->GetMinDamage(), ItemBase->GetMaxDamage()),
                            GetOwner()->GetInstigatorController(),this, DamageType);
		
}

void APlayerCharacter::LogPlayerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Player state is : %i"), PlayerState);
}

void APlayerCharacter::SetPlayerState(EPlayerState inpPlayerState)
{
	PlayerState = inpPlayerState;
}

void APlayerCharacter::ResetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}
