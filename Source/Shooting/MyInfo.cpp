// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInfo.h"

UMyInfo::UMyInfo(const FObjectInitializer& ObjectInitalizer) : Super(ObjectInitalizer)
{
	if (progressBar)
	{
		progressBar->SetPercent(0.0f);
	}
}

void UMyInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (btnReset)
		btnReset->OnClicked.AddDynamic(this, &UMyInfo::ResetCount);
}

void UMyInfo::UpdateBulletCount(eBulletType bulletType, int32 count)
{
	FString msg = FString::FromInt(count);

	switch (bulletType)
	{
	case eBulletType::Normal:
		textBox1->SetText(FText::FromString(msg));
		break;
	case eBulletType::Charge:
		textBox2->SetText(FText::FromString(msg));
		break;
	case eBulletType::Divied:
		textBox3->SetText(FText::FromString(msg));
		break;
	case eBulletType::Reflex:
		textBox4->SetText(FText::FromString(msg));
		break;
	default:
		break;
	}
}

void UMyInfo::UpdateChargeBar(float percent)
{
	if (progressBar)
	{
		progressBar->SetPercent(percent);
	}
}

void UMyInfo::ResetCount()
{
	if (GetWorld() == nullptr)
		return;

	AShootingCharacter* character = Cast<AShootingCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (character)
	{
		character->ResetBulletCount();
	}
}
