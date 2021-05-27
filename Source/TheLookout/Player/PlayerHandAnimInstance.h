// By Adamo Maiorano (2021)

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerHandAnimInstance.generated.h"

UCLASS()
class THELOOKOUT_API UPlayerHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerHandAnimInstance( const FObjectInitializer& ObjectInitializer );

	UFUNCTION()
	virtual void SetGrabAmount( float value );

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Grab" )
	float GrabAmount;
};
