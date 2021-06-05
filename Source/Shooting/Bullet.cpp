// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine/Classes/Components/SphereComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_mesh = CreateMesh();
	m_arrow = CreateArrow("Arrow1");
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

UStaticMeshComponent* ABullet::CreateMesh()
{
	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere"));
	if (Sphere.Succeeded())
	{
		mesh->SetStaticMesh(Sphere.Object);
		mesh->SetWorldScale3D(FVector(0.1));
		RootComponent = mesh;
	}
	return mesh;
}

UArrowComponent* ABullet::CreateArrow(FString name)
{
	UArrowComponent* arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(*name);
	if (arrow)
	{
		arrow->ArrowColor = FColor(255, 0, 0);
		arrow->ArrowSize = m_arrowSize;
		arrow->bTreatAsASprite = true;
		arrow->SetupAttachment(RootComponent);
		arrow->bIsScreenSizeScaled = true;
		arrow->bHiddenInGame = false;
	}

	return arrow;
}

void ABullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	Destroy();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_aliveTime += DeltaTime;

	if (m_aliveTime >= m_expireTime)
	{
		TimeOut();
		return;
	}
}

void ABullet::TimeOut()
{
	Destroy(true);
}

//////////////////////////////////////////////////////////////////////////
// 1. 일반 발사체
//////////////////////////////////////////////////////////////////////////
ANormalBullet::ANormalBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(L"ANormalBullet Create"));
}

void ANormalBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalBullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void ANormalBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * m_speed * DeltaTime;
	SetActorLocation(newLocation);
}

//////////////////////////////////////////////////////////////////////////
// 2. 충전 발사체
//////////////////////////////////////////////////////////////////////////
AChargeBullet::AChargeBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Red, FString::Printf(L"AChargeBullet Create"));

	// 화살표 크기.
	if (m_arrow)
		m_arrow->ArrowSize *= 3.0f;
	
	// 자동 소멸시간 변경
	m_expireTime = 5.0f;
}

void AChargeBullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void AChargeBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AChargeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * m_speed * DeltaTime;
	SetActorLocation(newLocation);
}

//////////////////////////////////////////////////////////////////////////
// 3. 분열 발사체
//////////////////////////////////////////////////////////////////////////
ADividedBullet::ADividedBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Red, FString::Printf(L"ADividedBullet Create"));

	// 양방향 45도 화살표 추가
	m_arrow2 = CreateArrow("Arrow2");
	m_arrow3 = CreateArrow("Arrow3");

	if (m_arrow2)
		m_arrow2->AddLocalRotation(FRotator(0.f, 45.f, 0.f));

	if (m_arrow3)
		m_arrow3->AddLocalRotation(FRotator(0.f, -45.f, 0.f));
}

void ADividedBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ADividedBullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void ADividedBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * m_speed * DeltaTime;
	SetActorLocation(newLocation);
}

void ADividedBullet::TimeOut()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
		return;

	world->SpawnActor<ANormalBullet>(ANormalBullet::StaticClass(), GetActorLocation(), m_arrow->GetComponentRotation());
	world->SpawnActor<ANormalBullet>(ANormalBullet::StaticClass(), GetActorLocation(), m_arrow3->GetComponentRotation());
	world->SpawnActor<ANormalBullet>(ANormalBullet::StaticClass(), GetActorLocation(), m_arrow2->GetComponentRotation());

	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// 4. 반사 발사체
//////////////////////////////////////////////////////////////////////////
AReflexBullet::AReflexBullet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Red, FString::Printf(L"AReflexBullet Create"));

	// 화살표 색깔 변경.
	if (m_arrow)
	{
		m_arrow->ArrowColor = FColor(0, 0, 255);
	}
}

void AReflexBullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	SetActorRotation(GetActorRotation() * -1);
}

void AReflexBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AReflexBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * m_speed * DeltaTime;
	SetActorLocation(newLocation);
}