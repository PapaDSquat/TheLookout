// By Adamo Maiorano (2021)

#include "VRLookoutPlayerPawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "VRMotionControllerComponent.h"
#include "Actors/Ball.h"

AVRLookoutPlayerPawn::AVRLookoutPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateComponents();
}

void AVRLookoutPlayerPawn::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAction( "GrabLeftHand", IE_Pressed, this, &AVRLookoutPlayerPawn::OnLeftHandGrabPress );
	PlayerInputComponent->BindAction( "GrabLeftHand", IE_Released, this, &AVRLookoutPlayerPawn::OnLeftHandGrabRelease );
	PlayerInputComponent->BindAction( "GrabRightHand", IE_Pressed, this, &AVRLookoutPlayerPawn::OnRightHandGrabPress );
	PlayerInputComponent->BindAction( "GrabRightHand", IE_Released, this, &AVRLookoutPlayerPawn::OnRightHandGrabRelease );
}

void AVRLookoutPlayerPawn::CreateComponents()
{
	RootComponent = CreateDefaultSubobject< USceneComponent >( TEXT( "SceneRoot" ) );

	PhysicsHandle = CreateDefaultSubobject< UPhysicsHandleComponent >( TEXT( "PhysicsHandle" ) );

	USceneComponent* cameraRoot = CreateDefaultSubobject< USceneComponent >( TEXT( "CameraRoot" ) );
	{
		cameraRoot->SetupAttachment( RootComponent );
		cameraRoot->SetRelativeLocationAndRotation( FVector::ZeroVector, FQuat::Identity );
		cameraRoot->SetRelativeScale3D( FVector::OneVector );
	}

	UCameraComponent* cameraComponent = CreateDefaultSubobject< UCameraComponent >( TEXT( "Camera" ) );
	{
		cameraComponent->SetupAttachment( cameraRoot );
		cameraComponent->SetRelativeLocationAndRotation( FVector::ZeroVector, FQuat::Identity );
		cameraComponent->SetRelativeScale3D( FVector::OneVector );
	}

	LeftHand = CreateDefaultSubobject< UVRMotionControllerComponent >( TEXT( "LeftHand" ) );
	{
		LeftHand->SetupAttachment( cameraRoot );
		LeftHand->SetupHand( EVRHandSide::Left );
		LeftHand->SetRelativeLocationAndRotation( FVector::ZeroVector, FQuat::Identity );
		LeftHand->SetRelativeScale3D( FVector::OneVector );
	}

	RightHand = CreateDefaultSubobject< UVRMotionControllerComponent >( TEXT( "RightHand" ) );
	{
		RightHand->SetupAttachment( cameraRoot );
		RightHand->SetupHand( EVRHandSide::Right );
		RightHand->SetRelativeLocationAndRotation( FVector::ZeroVector, FQuat::Identity );
		RightHand->SetRelativeScale3D( FVector::OneVector );
	}
}

bool AVRLookoutPlayerPawn::TryGrabBall( UVRMotionControllerComponent* handComponent )
{
	if( handComponent && !handComponent->HeldBall && handComponent->TargetBall )
	{
		handComponent->GrabTargetBall( GetRootComponent() );
		return true;
	}
	return false;
}

bool AVRLookoutPlayerPawn::TryReleaseBall( UVRMotionControllerComponent* handComponent )
{
	if( handComponent && handComponent->HeldBall )
	{
		handComponent->ReleaseHeldBall();
		return true;
	}
	return false;
}

void AVRLookoutPlayerPawn::OnLeftHandGrabPress_Implementation()
{
	if( LeftHand )
	{
		if( !TryGrabBall( LeftHand ) )
		{
			LeftHand->SetGrabAmount( 1.0f );
		}
	}
}
void AVRLookoutPlayerPawn::OnLeftHandGrabRelease_Implementation()
{
	if( LeftHand )
	{
		if( !TryReleaseBall( LeftHand ) )
		{
			LeftHand->SetGrabAmount( 0.0f );
		}
	}
}

void AVRLookoutPlayerPawn::OnRightHandGrabPress_Implementation()
{
	if( RightHand )
	{
		if( !TryGrabBall( RightHand ) )
		{
			RightHand->SetGrabAmount( 1.0f );
		}
	}
}
void AVRLookoutPlayerPawn::OnRightHandGrabRelease_Implementation()
{
	if( RightHand )
	{
		if( !TryReleaseBall( RightHand ) )
		{
			RightHand->SetGrabAmount( 0.0f );
		}
	}
}
