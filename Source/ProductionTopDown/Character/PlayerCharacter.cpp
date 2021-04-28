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
#include "FirstBoss.h"
//#include "ToolContextInterfaces.h"

#include "Engine/LevelStreaming.h"
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
	//Weapon->SetupAttachment(CharacterMesh, TEXT("WeaponSocket"));
	
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
	// Never call this function direcly. Let the healthcomponent call it.
	// If something needs to kill instantly use applydmg with a really big number :)
	
	if(PlayerState != EPlayerState::Dead)BPTriggerDeath(); // death message
	SetPlayerState(EPlayerState::Dead);
	
	//Super::TriggerDeath(); // particles and physics
	
	Cast<APawn>(this)->DisableInput(CharacterController);
	//if(CharacterController)GetOwner()->DisableInput(CharacterController);
	
	//UE_LOG(LogTemp, Warning, TEXT("Player died"));

}

void APlayerCharacter::OnLevelLoaded()
{
	NumberOfStreamingLevels--;
	if (NumberOfStreamingLevels == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("LEVEL LOADED"))
		EnableInput(Cast<APlayerController>(GetController()));
		GetCharacterMovement()->GravityScale = 1.f;
	}
	
}

void APlayerCharacter::HandleLevelStreamLoading()
{
	StreamingLevels.Empty();
	NumberOfStreamingLevels = 0;
	StreamingLevels = GetWorld()->GetStreamingLevels();
	DisableInput(Cast<APlayerController>(GetController()));
	GetCharacterMovement()->GravityScale = 0.f;
	
	for (auto StreamingLevel : StreamingLevels)
	{
		if (StreamingLevel)
		{
			NumberOfStreamingLevels++;
			StreamingLevel->OnLevelLoaded.AddDynamic(this, &APlayerCharacter::OnLevelLoaded);
		}
		
	}
	bool bLevelsLoaded{false};
	for (auto StreamingLevel : StreamingLevels)
	{
		if (StreamingLevel && StreamingLevel->IsLevelLoaded())
		{
			bLevelsLoaded = true;
		}
	}
	if (bLevelsLoaded)
	{
		EnableInput(Cast<APlayerController>(GetController()));
		GetCharacterMovement()->GravityScale = 1.f;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HandleLevelStreamLoading();

	InputVector = FVector(0,0,0);
	
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
	OnInventoryChange();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorldTimerManager().ClearTimer(AttackChargeTimerHandle);
	GetWorldTimerManager().ClearTimer(DashCooldownHandle);
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	GetWorldTimerManager().ClearTimer(LightOverLapEventHandle);
	GetWorldTimerManager().ClearTimer(LightMovingHandle);
	GetWorldTimerManager().ClearTimer(HeavyOverLapEventHandle);
	GetWorldTimerManager().ClearTimer(HeavyMovingHandle);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerState == EPlayerState::Dead) return;
		
	InputVector = ((GetActorForwardVector()*ForwardFloat)+(GetActorRightVector()*SideWaysFloat)).GetSafeNormal2D();
	if(InputVector != FVector().ZeroVector) LastInput = InputVector;
	
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
	
	if(GetMovementComponent()->IsFalling()) SetPlayerState(EPlayerState::Jumping);
	else if(PlayerState == EPlayerState::Jumping && !GetMovementComponent()->IsFalling()) SetPlayerState(EPlayerState::Moving);
	
	switch (GetPlayerState())
	{
	case EPlayerState::Attacking:
		//attack state
		break;
	case EPlayerState::Dashing:
		//dashing state
		break;
	case EPlayerState::Moving:
		WalkSpeedFloat = GetVelocity().Size2D();
		RotateCharacter();
		break;
	case EPlayerState::Pushing:
		RotateCharacter();
		PushObject(GetActorInFront());
		break;
	case EPlayerState::Dragging:
		DragObject(GetActorInFront());
	case EPlayerState::Charge:
		RotateCharacter();
		break;
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

/*
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
*/

void APlayerCharacter::DashEvent()
{
	//Dash Animation and particles
	if (GetPlayerState() == EPlayerState::Moving && bCanDash || GetPlayerState() == EPlayerState::Jumping && bCanDash)
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
	
	//LogPlayerState();
	// fix bug if you dash from ledge.
	
	GetCharacterMovement()->FallingLateralFriction = 8;
	//teleport player towards last direction
	//FVector DashDirection = LastDirection.GetSafeNormal()*DashDistance;	

	if(InputVector != FVector::ZeroVector)
		LaunchCharacter(InputVector*DashDistance, true , false);
	/*
	else
	{
		const FVector DashVector = NullRotation.Vector();
		LaunchCharacter(DashVector.GetSafeNormal2D()*DashDistance,true ,false);
	}
	*/
	//delay until dash is finish
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, [this]() {
		//code who runs after delay time
		SetPlayerState(EPlayerState::Moving);
		//LogPlayerState();
		GetCharacterMovement()->FallingLateralFriction = 0;
		//GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
    }, DashTimer, 0.f);


	//particle and sounds
	if (DashSound)
		UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
	if(DashParticle)
	{
	
		//const FVector SystemLocation = GetMesh()->GetSocketLocation("DashParticleSocket");
		const FVector SystemLocation = GetActorLocation() + (InputVector * 200);
		//const FRotator SystemRotation = GetMesh()->GetSocketRotation("DashParticleSocket");
		const FRotator SystemRotation = InputVector.Rotation();
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
	
	//delay between dashes.
	GetWorld()->GetTimerManager().SetTimer(DashCooldownHandle, [this]()
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
	if (InventoryComponent && InventoryComponent->GetItemObject() && PlayerState == EPlayerState::Moving)
	{
		//Charge animation
		PlayerState = EPlayerState::Charge;
		
		AItemBase* Item = InventoryComponent->GetItemObject();
		if(Item && Item->IsWeapon())
		{
			StartAttackTime = UGameplayStatics::GetTimeSeconds(GetWorld());
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed*0.2f;
			
			//spawn particle when charge done
			GetWorld()->GetTimerManager().SetTimer(AttackChargeTimerHandle, [this]()
            {
			if(AttackRangeComponent){
			const FVector SystemLocation = GetMesh()->GetSocketLocation("WeaponSocket");
	        const FRotator SystemRotation = GetMesh()->GetSocketRotation("WeaponSocket");
	        const FVector SystemScale = GetMesh()->GetSocketTransform("WeaponSocket").GetScale3D();
	        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	        GetWorld(),
	        AttackChargeTimerParticle,
	        SystemLocation,
	        SystemRotation,
	        SystemScale,
	        true,
	        true,
	        ENCPoolMethod::AutoRelease,
	        true
			);}
				
			if(AttackChargeTimerSound)UGameplayStatics::SpawnSoundAtLocation(GetWorld(),AttackChargeTimerSound, GetActorLocation(),GetActorRotation());
				
            }, InventoryComponent->GetItemObject()->GetAttackHeavyChargeTime(), 0.f);
		
		}
	}
}

void APlayerCharacter::StopAttackTimer()
{
	if (InventoryComponent && InventoryComponent->GetItemObject() && PlayerState == EPlayerState::Charge)
	{
		//stop Charge Animation
		PlayerState = EPlayerState::Moving;
		
		AItemBase* Item = InventoryComponent->GetItemObject();
		if(Item && Item->IsWeapon())
		{
			StopAttackTime = UGameplayStatics::GetTimeSeconds(GetWorld());
			ResetWalkSpeed();
			CalcAttackType();

			//stops timer
			GetWorld()->GetTimerManager().ClearTimer(AttackChargeTimerHandle);
		}
	}
}

void APlayerCharacter::CalcAttackType()
{

	
	if(InventoryComponent && InventoryComponent->GetItemObject())
	{
		
		//hold attack to heavy attack
		const float AttackHoldSeconds = StopAttackTime-StartAttackTime;
		if(PlayerState != EPlayerState::Moving) return;
		if(AttackHoldSeconds + 0.02f <= InventoryComponent->GetItemObject()->GetAttackHeavyChargeTime())
		{
			//makes Heavy weapon cant use light attack
			//if(InventoryComponent && InventoryComponent->GetItemObject() && InventoryComponent->GetItemObject()->GetItemName() == "The YEEEETEEER!") return;
			if(InventoryComponent->GetItemObject()->IsHeavy()) return; 
			
			if(!Super::Attack()) return;
			RotateCharToMouse();
			LightAttack();
			
		}
		else
		{
			//drain double Stamina
			if(!Super::Attack()) return;
			if(!Super::Attack()) return;
			RotateCharToMouse();
			//HeavyAttack();
			DoubleHeavyAttack();
		}
	}
}

void APlayerCharacter::LightAttack()
{
	
	SetPlayerState(EPlayerState::Attacking);
	if(InventoryComponent &&  InventoryComponent->GetItemObject())
	{
		const FVector BoxSize{140,140,50};
		AttackRangeComponent->SetBoxExtent(BoxSize,true);
		//SetBoxSize
		
		bHeavyAttack = false;
		
		if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(true);

		if (AttackSounds.Num() > 0)
		{
			const int32 RandSoundNum = FMath::RandRange(0, AttackSounds.Num()-1);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSounds[RandSoundNum], GetActorLocation(), GetActorRotation());
		}
		
		//if(LightAttackSound)UGameplayStatics::PlaySoundAtLocation(GetWorld(), LightAttackSound, GetActorLocation(), GetActorRotation());
		
		
			if(InventoryComponent->GetItemObject()->LightAttackEffect)
			{
				
			
			const FVector SystemLocation = GetMesh()->GetSocketLocation("AttackParticleSocket");
			const FRotator SystemRotation = GetMesh()->GetSocketRotation("AttackParticleSocket");
			const FVector SystemScale = GetMesh()->GetSocketTransform("AttackParticleSocket").GetScale3D();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	        GetWorld(),
			InventoryComponent->GetItemObject()->LightAttackEffect,
	        SystemLocation,
	        SystemRotation,
	        SystemScale,
	        true,
	        true,
	        ENCPoolMethod::AutoRelease,
	        true
	        );
		}
			
		

		
		GetWorld()->GetTimerManager().SetTimer(LightOverLapEventHandle, [this]() {
	        //code who runs after delay time
	        if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(false);
	    }, 0.05f, 0);

		GetWorld()->GetTimerManager().SetTimer(LightMovingHandle, [this]() {
	        //code who runs after delay time
	        SetPlayerState(EPlayerState::Moving);
	    }, InventoryComponent->GetItemObject()->GetAttackDelay(), 0);
	}
}

void APlayerCharacter::HeavyAttack()
{

	SetPlayerState(EPlayerState::Attacking);
	
	if(InventoryComponent && InventoryComponent->GetItemObject())
	{
		
		const FVector BoxSize{140,140,50};
		AttackRangeComponent->SetBoxExtent(BoxSize,true);
		//SetBoxRange
		
		bHeavyAttack = true;
		
		if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(true);

		if(HeavyAttackSound)UGameplayStatics::PlaySoundAtLocation(GetWorld(), HeavyAttackSound, GetActorLocation(), GetActorRotation());
		
		
			if(InventoryComponent->GetItemObject()->HeavyAttackEffect)
			{
				
			
			const FVector SystemLocation = GetMesh()->GetSocketLocation("AttackParticleSocket");
			const FRotator SystemRotation = GetMesh()->GetSocketRotation("AttackParticleSocket");
			const FVector SystemScale = GetMesh()->GetSocketTransform("AttackParticleSocket").GetScale3D();
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	            GetWorld(),
	            InventoryComponent->GetItemObject()->HeavyAttackEffect,
	            SystemLocation,
	            SystemRotation,
	            SystemScale,
	            true,
	            true,
	            ENCPoolMethod::AutoRelease,
	            true
	            );
			}
		
		
		GetWorld()->GetTimerManager().SetTimer(HeavyOverLapEventHandle, [this]() {
	        //code who runs after delay time
	        if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(false);
			UE_LOG(LogTemp,Warning, TEXT("Overlap False first time"));
			
	    }, 0.05f, 0);
		
		GetWorld()->GetTimerManager().SetTimer(HeavyMovingHandle, [this]() {
	        //code who runs after delay time
	        SetPlayerState(EPlayerState::Moving);
	    }, InventoryComponent->GetItemObject()->GetAttackDelay(), 0.f);
	}
}

void APlayerCharacter::DoubleHeavyAttack()
{
	SetPlayerState(EPlayerState::HeavyAttack);
	if(InventoryComponent && InventoryComponent->GetItemObject())
	{
		
	
	const FVector BoxSize{160,140,50};
	AttackRangeComponent->SetBoxExtent(BoxSize,true);
	//SetBoxRange
		
	bHeavyAttack = true;
		
	if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(true);
	UE_LOG(LogTemp,Warning, TEXT("Overlap true first time"));
	
	if(HeavyAttackSound)UGameplayStatics::PlaySoundAtLocation(GetWorld(), HeavyAttackSound, GetActorLocation(), GetActorRotation());

	
	if(InventoryComponent->GetItemObject()->HeavyAttackEffect)
	{
				
		
		const FVector SystemLocation = GetMesh()->GetSocketLocation("HeavyParticle1");
		const FRotator SystemRotation = GetMesh()->GetSocketRotation("HeavyParticle1");
		const FVector SystemScale = GetMesh()->GetSocketTransform("HeavyParticle1").GetScale3D();
			
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            InventoryComponent->GetItemObject()->HeavyAttackEffect,
            SystemLocation,
            SystemRotation,
            SystemScale,
            true,
            true,
            ENCPoolMethod::AutoRelease,
            true
            );
	}
	
	GetWorld()->GetTimerManager().SetTimer(HeavyOverLapEventHandle, [this]() {
                //code who runs after delay time
                if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(false);
			UE_LOG(LogTemp,Warning, TEXT("Overlap False first time"));
            }, 0.05f, 0);
	
	GetWorld()->GetTimerManager().SetTimer(HeavyParticle2, [this]() {
		
		
		if(HeavyAttackSound)UGameplayStatics::PlaySoundAtLocation(GetWorld(), HeavyAttackSound, GetActorLocation(), GetActorRotation());
		
		if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(true);
		UE_LOG(LogTemp,Warning, TEXT("Overlap true second time"));
		
		if(InventoryComponent->GetItemObject()->HeavyAttackEffect)
		{

            const FVector SystemLocation = GetMesh()->GetSocketLocation("HeavyParticle2");
            const FRotator SystemRotation = GetMesh()->GetSocketRotation("HeavyParticle2");
            const FVector SystemScale = GetMesh()->GetSocketTransform("HeavyParticle2").GetScale3D();
			
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                InventoryComponent->GetItemObject()->HeavyAttackEffect,
                SystemLocation,
                SystemRotation,
                SystemScale,
                true,
                true,
                ENCPoolMethod::AutoRelease,
                true
                );
        }
		
		if(HeavyAttackSound)UGameplayStatics::PlaySoundAtLocation(GetWorld(), HeavyAttackSound, GetActorLocation(), GetActorRotation());
		
        }, 0.2f, 0);
	
	GetWorld()->GetTimerManager().SetTimer(HeavyOverLapEventHandle2, [this]() {
            //code who runs after delay time
            if(AttackRangeComponent)AttackRangeComponent->SetGenerateOverlapEvents(false);
		UE_LOG(LogTemp,Warning, TEXT("Overlap False second time"));
        }, 0.25f, 0);
		
	GetWorld()->GetTimerManager().SetTimer(HeavyMovingHandle, [this]() {
        //code who runs after delay time
        SetPlayerState(EPlayerState::Moving);
    }, InventoryComponent->GetItemObject()->GetAttackDelay(), 0.f);
	}
}

float APlayerCharacter::GetAttackDamage()
{
	float Damage{0};
	
	if (bHeavyAttack && InventoryComponent && InventoryComponent->GetItemObject() &&  InventoryComponent->GetItemObject()->IsWeapon())
	{
		//heavy attack damage
		Damage = FMath::RandRange(
        InventoryComponent->GetItemObject()->GetMinDamage(),
        InventoryComponent->GetItemObject()->GetMaxDamage()
        );
		Damage *= 1,5.f;
	}
	else if(InventoryComponent && InventoryComponent->GetItemObject() && InventoryComponent->GetItemObject()->IsWeapon())
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

	ForwardFloat = Value;
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
	
	SideWaysFloat = Value;
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
		
		NullRotation = InputVector.Rotation();
		NullRotation.Yaw -= 90;
		NullRotation.Pitch = 0;
		
		CharacterMesh->SetWorldRotation(MeshRotation);
	}
	if(CharacterMesh && GetVelocity() == FVector().ZeroVector)
	{
		FRotator LastInputRotation = LastInput.Rotation();
		LastInputRotation.Yaw -= 90;
		//UE_LOG(LogTemp, Error , TEXT("Inputrotation:  %s"), *LastInputRotation.ToString());
		CharacterMesh->SetWorldRotation(LastInputRotation);
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
	// lock guard because playercontroller and playercharacter is in the same tick group
	// so that they both can change the state with thread safety.
	FScopeLock Lock(&PlayerStateCriticalSection);
	PlayerState = inpPlayerState;
}

void APlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(InventoryComponent && InventoryComponent->GetItemObject())
	{
		
	if(OtherActor != this)
	{
		if(OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherComp->GetOwner()->IsA(ACharacterBase::StaticClass())
			|| !OtherActor->IsA(ACharacterBase::StaticClass()))
		{
			{
				UGameplayStatics::ApplyDamage(
                    OtherComp->GetOwner(),
                    GetAttackDamage(),
                    GetInstigatorController(),
                    this,
                    DamageType
                    );
				
			}

			
			
			if(bHeavyAttack)
			{
				FVector PushBackVector = (OtherComp->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
				PushBackVector *=2;
				if(OtherComp->GetOwner()->IsA(AFirstBoss::StaticClass())) PushBackVector = FVector().ZeroVector; // removes Knockback on boss
				ACharacterBase* Characterbaseptr = Cast<ACharacterBase>(OtherComp->GetOwner());
				if(Characterbaseptr)Characterbaseptr->LaunchCharacter(PushBackVector*InventoryComponent->GetItemObject()->GetKnockbackAmount(), true, false);
				if(OtherActor->FindComponentByClass(UHealthComponent::StaticClass()))InventoryComponent->GetItemObject()->Durability -=2;
			}
			else
			{
				FVector PushBackVector = (OtherComp->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
				if(OtherComp->GetOwner()->IsA(AFirstBoss::StaticClass())) PushBackVector = FVector().ZeroVector; // removes Knockback on boss
				ACharacterBase* Characterbaseptr = Cast<ACharacterBase>(OtherComp->GetOwner());
				if(Characterbaseptr)Characterbaseptr->LaunchCharacter(PushBackVector*InventoryComponent->GetItemObject()->GetKnockbackAmount(), true, false);
				if(OtherActor->FindComponentByClass(UHealthComponent::StaticClass()))InventoryComponent->GetItemObject()->Durability -=1;
			}
			if(InventoryComponent->GetItemObject()->Durability <= 0) InventoryComponent->DestroyWeapon();
		}
		}
	}
}

void APlayerCharacter::ResetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}
