// By Adamo Maiorano (2021)

#include "VRLookoutPlayerPawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "VRMotionControllerComponent.h"
#include "Actors/Ball.h"

AVRLookoutPlayerPawn::AVRLookoutPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AVRLookoutPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> handComponents;
	GetComponents( UVRMotionControllerComponent::StaticClass(), handComponents, true );
	for( const auto& comp : handComponents )
	{
		if( auto handComp = Cast< UVRMotionControllerComponent >( comp ) )
		{
			if( handComp->HandSide == EVRHandSide::Left )
			{
				LeftHand = handComp;
			}
			else if( handComp->HandSide == EVRHandSide::Right )
			{
				RightHand = handComp;
			}
		}
	}
}

void AVRLookoutPlayerPawn::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAction( "GrabLeftHand", IE_Pressed, this, &AVRLookoutPlayerPawn::OnLeftHandGrabPress );
	PlayerInputComponent->BindAction( "GrabLeftHand", IE_Released, this, &AVRLookoutPlayerPawn::OnLeftHandGrabRelease );
	PlayerInputComponent->BindAction( "GrabRightHand", IE_Pressed, this, &AVRLookoutPlayerPawn::OnRightHandGrabPress );
	PlayerInputComponent->BindAction( "GrabRightHand", IE_Released, this, &AVRLookoutPlayerPawn::OnRightHandGrabRelease );
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
