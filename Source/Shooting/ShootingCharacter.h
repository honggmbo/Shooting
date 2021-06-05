// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShootingCharacter.generated.h"

enum class eBulletType
{
	Normal,
	Charge,
	Divied,
	Reflex,
};

class ABullet;

UCLASS(config=Game)
class AShootingCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface



	// Called every frame
	virtual void Tick(float DeltaTime);

	UFUNCTION()
		virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;
public:
	AShootingCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

public:
	FVector GetBulletStartPossition();

	void BulletA_Pressed();
	void BulletA_Released();
	void BulletB_Pressed();
	void BulletB_Released();

	void CreateBullet(eBulletType type);
	void NormalBullet();
	void ChargeBullet();
	void DiviedBullet();
	void ReflexBullet();

	void ResetKeyInput();

protected:
	const float m_bulletPosX = 20.0f;
	const float m_bulletPosY = 50.0f;

	bool m_bKeyPress_Q = false;
	bool m_bKeyPress_W = false;
	float m_keyPressTime_Q = 0;
};
