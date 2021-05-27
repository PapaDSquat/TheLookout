// By Adamo Maiorano (2021)

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "VRMotionControllerComponent.generated.h"

class UPhysicsHandleComponent;
class UMotionControllerComponent;
class UPlayerHandAnimInstance;
class IGrabbableInterface;
class ABall;

UENUM()
enum EVRHandSide
{
	Left     UMETA( DisplayName = "LeftHand" ),
	Right    UMETA( DisplayName = "RightHand" ),
};

// TODO: Might be better to move this fuctionality to a custom actor class

UCLASS()
class THELOOKOUT_API UVRMotionControllerComponent : public UMotionControllerComponent
{
	GENERATED_BODY()

public:
	UVRMotionControllerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Public Interface
	void SetupHand( EVRHandSide side );
	void GrabTargetBall( USceneComponent* parentComponent );
	void ReleaseHeldBall();
	void SetGrabAmount( float amount );

	UPROPERTY( EditDefaultsOnly, Category = "Hand Settings" )
	float GrabMaxDistance = 35.f;

	UPROPERTY( EditDefaultsOnly, Category = "Hand Settings" )
	float ThrowStrengthMultiplier = 65.f;

	UPROPERTY( EditDefaultsOnly, Category = Components )
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY( BlueprintReadOnly, Category = Hand )
	TEnumAsByte< EVRHandSide > HandSide;

	UPROPERTY( BlueprintReadOnly, Category = Grab )
	ABall* HeldBall;

	UPROPERTY( BlueprintReadOnly, Category = Grab )
	UPrimitiveComponent* HeldBallComponent;

	UPROPERTY( BlueprintReadOnly, Category = Grab )
	ABall* TargetBall;

	UPROPERTY( BlueprintReadOnly, Category = Grab )
	UPrimitiveComponent* TargetBallComponent;

private:
	// Setup
	void CreateComponents();
	USkeletalMeshComponent* CreateSkeletalMesh( UMotionControllerComponent* parentComponent );
	void SetupAnimationBlueprint( USkeletalMeshComponent* skeletalMesh );

	// Targetting
	void SearchForTarget();
	void SetNewTargetBall( ABall* ball, UPrimitiveComponent* ballComponent );
	void ClearTargetBall();

	// Helpers
	FVector GetGrabSocketLocation() const;

	USkeletalMeshComponent* m_skeletalMesh;
	UPlayerHandAnimInstance* m_animBPInstance;
	float m_grabAmount;
};
