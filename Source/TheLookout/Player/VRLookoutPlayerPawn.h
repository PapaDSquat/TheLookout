// By Adamo Maiorano (2021)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRLookoutPlayerPawn.generated.h"

class UVRMotionControllerComponent;
class UPhysicsHandleComponent;

UCLASS()
class THELOOKOUT_API AVRLookoutPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AVRLookoutPlayerPawn();

public:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	// Events
	UFUNCTION( BlueprintNativeEvent, Category = "Input" )
	void OnLeftHandGrabPress();
	UFUNCTION( BlueprintNativeEvent, Category = "Input" )
	void OnLeftHandGrabRelease();
	UFUNCTION( BlueprintNativeEvent, Category = "Input" )
	void OnRightHandGrabPress();
	UFUNCTION( BlueprintNativeEvent, Category = "Input" )
	void OnRightHandGrabRelease();

	UPROPERTY( EditDefaultsOnly, Category = "Components" )
	UPhysicsHandleComponent* PhysicsHandle;

private:
	bool TryGrabBall( UVRMotionControllerComponent* handComponent );
	bool TryReleaseBall( UVRMotionControllerComponent* handComponent );

	UVRMotionControllerComponent* LeftHand;
	UVRMotionControllerComponent* RightHand;
};
