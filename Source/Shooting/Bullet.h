// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/StaticMeshActor.h>
#include "Components/ArrowComponent.h"
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
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime);

public:

	virtual void TimeOut();

	UFUNCTION()
		virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;

	UStaticMeshComponent* CreateMesh();
	UArrowComponent* CreateArrow(FString name);

protected:
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_mesh;

	UArrowComponent* m_arrow = nullptr;

	float m_speed = 100.0f;	// √ ¥Á 100
	const float m_arrowSize = 3.0f;
	float m_expireTime = 3.0f;
	float m_aliveTime = 0.0f;

};

// ANormalBullet
UCLASS()
class ANormalBullet : public ABullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANormalBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;

};

// AChargeBullet
UCLASS()
class AChargeBullet : public ABullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChargeBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;

};

// ADividedBullet
UCLASS()
class ADividedBullet : public ABullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADividedBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;
	
	virtual void TimeOut();

protected:
	class UArrowComponent* m_arrow2;
	class UArrowComponent* m_arrow3;

};

// AReflexBullet
UCLASS()
class AReflexBullet : public ABullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReflexBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;

};