// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"

AMyHUD::AMyHUD()
{

}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MyInfoClass)
	{
		myInfo = CreateWidget<UMyInfo>(GetWorld(), MyInfoClass);
		if (myInfo)
		{
			myInfo->AddToViewport();
		}
	}
}

void AMyHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMyHUD::UpdateBulletCount(eBulletType bulletType, int32 count)
{
	if (myInfo)
	{
		myInfo->UpdateBulletCount(bulletType, count);
	}
}

void AMyHUD::UpdateChargeBar(float percent)
{
	if (myInfo)
	{
		myInfo->UpdateChargeBar(percent);
	}
}
