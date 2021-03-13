// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProductionTopDown/Components/InventoryComponent.h"

#include "kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "ProductionTopDown/Components/InteractComponent.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
	
	AttackRangeComponent = CreateDefaultSubobject<UBoxComponent>("Attack Range Component");
	AttackRangeComponent->SetupAttachment(CharacterMesh, TEXT("Attack Range"));
	
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(CharacterMesh, TEXT("WeaponSocket"));
	
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
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//start in moving state
	PlayerState = EPlayerState::Moving;
	LogPlayerState();
	
	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();
	
	//testing purposes
	if(Weapon)
	{
		//Weapon->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnWeaponHit);
		Weapon->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlap);
		EquipWeaponFromInv(Weapon);
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
	
	if(InventoryComponent->GetItemObject()!= nullptr && InventoryComponent->GetItemObject()->IsWeapon() && PlayerState == EPlayerState::Moving)
	{
		Attack();
	}
	
}

bool APlayerCharacter::Attack()
{
	// returns false if there is not enough stamina
	if (!Super::Attack()) return false;
	// Attack code here
	//rotates char to cursor
	RotateCharToMouse();

	PlayerState = EPlayerState::Attacking;
	
	
	//makes you walk slower while attacking
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed*0.2f;
	
	//PlayerState = EPlayerState::Attacking;
	LogPlayerState();
	
	
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
	if (PlayerState == EPlayerState::Moving)
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

	//particle and sounds
	if (DashSound) UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
	if (DashParticle) UGameplayStatics::SpawnEmitterAtLocation(this, DashParticle, GetActorLocation());
	
	LogPlayerState();
	
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
    }, DashTimer, 0);
	
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
	//EquipWeapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
}

void APlayerCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Other component hit : %s"), *OtherActor->GetHumanReadableName())
	AItemBase* ItemBase = FindComponentByClass<UInventoryComponent>()->GetItemObject();
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
