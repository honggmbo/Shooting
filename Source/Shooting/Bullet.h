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
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* m_arrow;

	UFUNCTION()
		virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;

	void CreateMesh();
	void CreateArrow();

protected:
	bool m_bStart = false;
	float m_speed = 100.0f;	// √ ¥Á 100
	float m_arrowSize = 3.0f;
	float m_expireTime = 3.0f;
	float m_aliveTime = 0.0f;
	FVector m_direction;


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

};