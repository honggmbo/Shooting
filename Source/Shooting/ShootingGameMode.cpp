// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingGameMode.h"
#include "ShootingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootingGameMode::AShootingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
