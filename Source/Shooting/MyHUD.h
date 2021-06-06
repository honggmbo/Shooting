// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "ShootingCharacter.h"
#include "MyInfo.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void UpdateBulletCount(eBulletType bulletType, int32 count);
	void UpdateChargeBar(float percent);

	UPROPERTY(EditDefaultsOnly, Category = "wedget")
		TSubclassOf<UUserWidget> MyInfoClass;

private:
	UPROPERTY()
		class UMyInfo* myInfo;
};
