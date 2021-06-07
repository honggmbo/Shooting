// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// 1. �Ϲ� �߻�ü
//////////////////////////////////////////////////////////////////////////
ANormalBullet::ANormalBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(L"ANormalBullet Create"));

	// ����ð� ����
	InitialLifeSpan = m_defaultExpireTime;

	// Static Mesh
	CreateStaticMesh();

	// Arrow
	m_arrow = CreateArrow("Arrow1");

	// Movement
	CreateMovement();
}

void ANormalBullet::CreateStaticMesh()
{
	m_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere"));
	if (Sphere.Succeeded())
	{
		m_staticMesh->SetStaticMesh(Sphere.Object);
		m_staticMesh->SetWorldScale3D(FVector(0.2));
		m_staticMesh->bUseDefaultCollision = true;
		m_staticMesh->SetNotifyRigidBodyCollision(true);
		m_staticMesh->SetCollisionProfileName(FName(TEXT("BlockAll")));
		m_staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		m_staticMesh->SetGenerateOverlapEvents(true);
		RootComponent = m_staticMesh;
	}
}

UArrowComponent* ANormalBullet::CreateArrow(FString name)
{
	UArrowComponent* arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(*name);
	if (arrow)
	{
		arrow->ArrowColor = FColor(255, 0, 0);
		arrow->ArrowSize = m_defaultArrowSize;
		arrow->bTreatAsASprite = true;
		arrow->bIsScreenSizeScaled = true;
		arrow->bHiddenInGame = false;
		arrow->SetupAttachment(RootComponent);
	}

	return arrow;
}

void ANormalBullet::CreateMovement()
{
	m_movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Move"));
	if (m_movement)
	{
		m_movement->InitialSpeed = m_defaultSpeed;
		m_movement->ProjectileGravityScale = 0.0f;
		m_movement->bRotationFollowsVelocity = true;
		m_movement->bShouldBounce = true;
		m_movement->Bounciness = m_defaultSpeed;
		m_movement->SetUpdatedComponent(m_staticMesh);
	}
}

void ANormalBullet::BeginPlay()
{
	Super::BeginPlay();

	m_radius = GetSimpleCollisionRadius();

	// �浹
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), m_actors);
}

void ANormalBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsExpired(DeltaTime))
		return;

	m_aliveTime += DeltaTime;
}

bool ANormalBullet::IsExpired(float curTime)
{
	return false;
}

void ANormalBullet::TimeOut()
{
	Destroy();
}

void ANormalBullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	ABullet* bullet = Cast<ABullet>(Other);
	if (bullet == nullptr)
		Destroy();
}

//////////////////////////////////////////////////////////////////////////
// 2. ���� �߻�ü
//////////////////////////////////////////////////////////////////////////
AChargeBullet::AChargeBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Red, FString::Printf(L"AChargeBullet Create"));

	// ȭ��ǥ ũ��.
	if (m_arrow)
		m_arrow->ArrowSize *= 3.0f;
	
	// �ڵ� �Ҹ�ð� ���� sec
	m_defaultExpireTime = 5.0f;
	InitialLifeSpan = m_defaultExpireTime;
}

//////////////////////////////////////////////////////////////////////////
// 3. �п� �߻�ü
//////////////////////////////////////////////////////////////////////////
ADividedBullet::ADividedBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Red, FString::Printf(L"ADividedBullet Create"));

	// ����� 45�� ȭ��ǥ �߰�
	m_arrow2 = CreateArrow("Arrow2");
	m_arrow3 = CreateArrow("Arrow3");

	if (m_arrow2)
		m_arrow2->AddLocalRotation(FRotator(0.f, 45.f, 0.f));

	if (m_arrow3)
		m_arrow3->AddLocalRotation(FRotator(0.f, -45.f, 0.f));
}

void ADividedBullet::TimeOut()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
		return;

	// �浹������ ��ǥ�� �̵����ش�.
	FVector location1 = GetActorLocation() + (m_arrow->GetForwardVector() * m_radius * 2);
	FVector location2 = GetActorLocation() + (m_arrow2->GetForwardVector() * m_radius * 2);
	FVector location3 = GetActorLocation() + (m_arrow3->GetForwardVector() * m_radius * 2);


	world->SpawnActor<ANormalBullet>(ANormalBullet::StaticClass(), location1, m_arrow->GetComponentRotation());
	world->SpawnActor<ANormalBullet>(ANormalBullet::StaticClass(), location2, m_arrow2->GetComponentRotation());
	world->SpawnActor<ANormalBullet>(ANormalBullet::StaticClass(), location3, m_arrow3->GetComponentRotation());

	Destroy();
}

bool ADividedBullet::IsExpired(float curTime)
{
	if (m_aliveTime + curTime > m_defaultExpireTime)
	{
		TimeOut();
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 4. �ݻ� �߻�ü
//////////////////////////////////////////////////////////////////////////
AReflexBullet::AReflexBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Red, FString::Printf(L"AReflexBullet Create"));

	// ȭ��ǥ ���� ����.
	if (m_arrow)
	{
		m_arrow->ArrowColor = FColor(0, 0, 255);
	}
}

void AReflexBullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	SetActorRotation(GetActorRotation() * -1);
	m_movement->Velocity *= -1;
}