// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include <Engine/Classes/GameFramework/ProjectileMovementComponent.h>
#include "Bullet.generated.h"

UCLASS()
class SHOOTING_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	

	virtual void Tick(float DeltaTime);
	virtual bool IsExpired(float curTime) { return false; }
	virtual void TimeOut() {}

protected:
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_staticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UArrowComponent* m_arrow = nullptr;
	
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* m_movement = nullptr;


	const float m_defaultSpeed = 100.0f;	// 초당 100
	const float m_defaultArrowSize = 3.0f;	// default
	float m_defaultExpireTime = 3.0f; // 자동 소멸시간 변경 sec
	float m_aliveTime = 0.0f;
	float m_radius = 0.0;

	TArray<AActor*> m_actors;
};

// ANormalBullet
UCLASS()
class ANormalBullet : public ABullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANormalBullet();

	void CreateStaticMesh();
	UArrowComponent* CreateArrow(FString name);
	void CreateMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsExpired(float curTime) override;
	virtual void TimeOut() override;
	virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;
};

// AChargeBullet
UCLASS()
class AChargeBullet : public ANormalBullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChargeBullet();

};

// ADividedBullet
UCLASS()
class ADividedBullet : public ANormalBullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADividedBullet();

public:

	virtual void TimeOut();

	virtual bool IsExpired(float curTime) override;

protected:
	class UArrowComponent* m_arrow2;
	class UArrowComponent* m_arrow3;

};

// AReflexBullet
UCLASS()
class AReflexBullet : public ANormalBullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReflexBullet();

public:

	virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;

};