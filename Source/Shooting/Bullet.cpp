// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine/Classes/Components/SphereComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateMesh();
	CreateArrow();
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	m_bStart = true;
}

void ABullet::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	Destroy(true);
}

void ABullet::CreateMesh()
{
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> cubBox(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> cubBox(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	if (cubBox.Succeeded())
	{
		m_mesh->SetStaticMesh(cubBox.Object);
		m_mesh->SetWorldScale3D(FVector(0.1));
		RootComponent = m_mesh;
	}

}

void ABullet::CreateArrow()
{
	m_arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (m_arrow)
	{
		m_arrow->ArrowColor = FColor(255, 0, 0);
		m_arrow->ArrowSize = m_arrowSize;
		m_arrow->bTreatAsASprite = true;
		m_arrow->SetupAttachment(RootComponent);
		m_arrow->bIsScreenSizeScaled = true;
		m_arrow->bHiddenInGame = false;
	}
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// ANormalBullet
//////////////////////////////////////////////////////////////////////////
ANormalBullet::ANormalBullet()
{

}

void ANormalBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalBullet::Tick(float DeltaTime)
{
	if (m_bStart == false)
		return;

	m_aliveTime += DeltaTime;

	if (m_aliveTime >= m_expireTime)
		Destroy(true);

	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * m_speed * DeltaTime;
	SetActorLocation(newLocation);
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(L"%s", *GetActorLocation().ToString()));

	//UE_LOG(LogTemp, Log, TEXT("possition %f %f %f"), newLocation.X, newLocation.Y, newLocation.Z);
}

//////////////////////////////////////////////////////////////////////////
// AChargeBullet
//////////////////////////////////////////////////////////////////////////
AChargeBullet::AChargeBullet()
{
	// 화살표 크기.
	if (m_arrow)
		m_arrow->ArrowSize *= 3.0f;
	
	// 자동 소멸시간 변경
	m_expireTime = 5.0f;
}

//////////////////////////////////////////////////////////////////////////
// ADividedBullet
//////////////////////////////////////////////////////////////////////////
ADividedBullet::ADividedBullet()
{
	m_expireTime = 5.0f;
}

void ADividedBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ADividedBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * m_speed * DeltaTime;
	SetActorLocation(newLocation);
}

//////////////////////////////////////////////////////////////////////////
// AReflexBullet
//////////////////////////////////////////////////////////////////////////
AReflexBullet::AReflexBullet()
{

}

void AReflexBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AReflexBullet::Tick(float DeltaTime)
{
	if (m_bStart == false)
		return;

	FVector newLocation = GetActorLocation();
	newLocation += GetActorForwardVector() * m_speed * DeltaTime;
	SetActorLocation(newLocation);
}
