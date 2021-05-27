// By Adamo Maiorano (2021)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TheLookoutGameStateBase.generated.h"

UCLASS()
class THELOOKOUT_API ATheLookoutGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Game )
	bool GameActive = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Game )
	bool GameJustPlayed = false;
};
