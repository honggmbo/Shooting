// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootingCharacter.h"
#include "Runtime/UMG/Public/UMG.h"
#include "MyInfo.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMyInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UMyInfo(const FObjectInitializer& ObjectInitalizer);
	
	void UpdateBulletCount(eBulletType bulletType, int32 count);
	void UpdateChargeBar(float percent);
	
	UFUNCTION(BlueprintCallable)
	void ResetCount();

protected:

	virtual void NativeConstruct() override;	// C++ À§Á¬ »ý¼ºÀÚ


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* textBox1;
	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* textBox2;
	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* textBox3;
	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* textBox4;
	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* progressBar;
	UPROPERTY(Meta = (BindWidget))
		class UCanvasPanel* canvasPanel;
	UPROPERTY(Meta = (BindWidget))
		class UButton* btnReset;
	
};
