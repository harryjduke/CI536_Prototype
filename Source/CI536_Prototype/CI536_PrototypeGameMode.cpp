// Copyright Epic Games, Inc. All Rights Reserved.

#include "CI536_PrototypeGameMode.h"
#include "CI536_PrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACI536_PrototypeGameMode::ACI536_PrototypeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
