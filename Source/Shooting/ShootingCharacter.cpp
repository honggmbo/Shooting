// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bullet.h"

AShootingCharacter::AShootingCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShootingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootingCharacter::MoveRight);

	// Bullet
	PlayerInputComponent->BindKey(EKeys::Q, IE_Pressed, this, &AShootingCharacter::BulletA_Pressed);
	PlayerInputComponent->BindKey(EKeys::Q, IE_Released, this, &AShootingCharacter::BulletA_Released);
	PlayerInputComponent->BindKey(EKeys::W, IE_Pressed, this, &AShootingCharacter::BulletB_Pressed);
	PlayerInputComponent->BindKey(EKeys::W, IE_Released, this, &AShootingCharacter::BulletB_Released);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AShootingCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AShootingCharacter::TouchStopped);
}

void AShootingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bKeyPress_Q)
	{
		m_keyPressTime_Q += DeltaTime;
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(L"%s", *GetActorLocation().ToString()));
}

void AShootingCharacter::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Hit"));
}

void AShootingCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void AShootingCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AShootingCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

FVector AShootingCharacter::GetBulletStartPossition()
{
	FVector direction = GetActorForwardVector();
	FVector location = GetActorLocation();

	float OutCollisionRadius;
	float OutCollisionHalfHeight;
	GetComponentsBoundingCylinder(OutCollisionRadius, OutCollisionHalfHeight);

	// 캐릭터 전방
	direction *= (OutCollisionRadius + m_bulletPosX);
	location += direction;
	
	// 캐릭터 발밑
	location.Z = location.Z - OutCollisionHalfHeight;
	location.Z += m_bulletPosY;

	return location;
}

void AShootingCharacter::ResetKeyInput()
{
	m_keyPressTime_Q = 0;
	m_bKeyPress_Q = false;
	m_bKeyPress_W = false;
}

void AShootingCharacter::BulletA_Pressed()
{
	if (m_bKeyPress_Q == false)
	{
		m_bKeyPress_Q = true;
	}
}

void AShootingCharacter::BulletA_Released()
{
	if (m_bKeyPress_Q == false)	// 분열 반사체
		return;

	if (m_keyPressTime_Q < 3.0f)
	{
		CreateBullet(eBulletType::Normal);
	}
	else
	{
		CreateBullet(eBulletType::Charge);
	}
}

void AShootingCharacter::BulletB_Pressed()
{
	m_bKeyPress_W = true;

	if (m_bKeyPress_Q == true && m_keyPressTime_Q < 1.0f)
	{
		m_bKeyPress_Q = false;
		m_keyPressTime_Q = 0;
		CreateBullet(eBulletType::Divied);
	}
}

void AShootingCharacter::BulletB_Released()
{
	if (m_bKeyPress_Q)
		return;

	if (m_bKeyPress_W == false)
		return;

	CreateBullet(eBulletType::Reflex);
}

void AShootingCharacter::CreateBullet(eBulletType type)
{
	ResetKeyInput();

	UWorld* world = GetWorld();
	if (world == nullptr)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	switch (type)	
	{
		case eBulletType::Normal:
			world->SpawnActor<ANormalBullet>(ANormalBullet::StaticClass(), GetBulletStartPossition(), GetActorRotation(), SpawnParams);
			break;
		case eBulletType::Charge:
			world->SpawnActor<AChargeBullet>(AChargeBullet::StaticClass(), GetBulletStartPossition(), GetActorRotation(), SpawnParams);
			break;
		case eBulletType::Divied:
			world->SpawnActor<ADividedBullet>(ADividedBullet::StaticClass(), GetBulletStartPossition(), GetActorRotation(), SpawnParams);
			break;
		case eBulletType::Reflex:
			world->SpawnActor<AReflexBullet>(AReflexBullet::StaticClass(), GetBulletStartPossition(), GetActorRotation(), SpawnParams);
			break;
		default:
			break;
	}
}