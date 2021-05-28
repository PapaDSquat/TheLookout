// By Adamo Maiorano (2021)

#include "Player/VRMotionControllerComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"
#include "PlayerHandAnimInstance.h"
#include "Interfaces/GrabbableInterface.h"
#include "Actors/Ball.h"

UVRMotionControllerComponent::UVRMotionControllerComponent()
{
	CreateComponents();
}

void UVRMotionControllerComponent::CreateComponents()
{
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>( TEXT( "PhysicsHandle" ) );
}

void UVRMotionControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	HandSide = MotionSource == FXRMotionControllerBase::LeftHandSourceId ? EVRHandSide::Left : EVRHandSide::Right;

	TArray< USceneComponent* > childComps;
	GetChildrenComponents( true, childComps );
	for( USceneComponent* comp : childComps )
	{
		if( auto skeletalMeshComp = Cast< USkeletalMeshComponent >( comp ) )
		{
			m_skeletalMesh = skeletalMeshComp;
			
			// TODO
			//SetupAnimationBlueprint( m_skeletalMesh );
			//m_animBPInstance = Cast< UPlayerHandAnimInstance >( m_skeletalMesh->GetAnimInstance() );
		}
	}
}

void UVRMotionControllerComponent::SetupAnimationBlueprint( USkeletalMeshComponent* skeletalMesh )
{
	// TODO: Editable property
	static ConstructorHelpers::FObjectFinder< UAnimBlueprint > HandAnimBP( TEXT( "AnimBlueprint'/Game/VirtualReality/Mannequin/Animations/RightGrip_AnimBP.RightGrip_AnimBP'" ) );
	if( HandAnimBP.Succeeded() )
	{
		skeletalMesh->AnimClass = HandAnimBP.Object->GetAnimBlueprintGeneratedClass();
		skeletalMesh->SetAnimationMode( EAnimationMode::AnimationBlueprint );
		skeletalMesh->SetAnimInstanceClass( HandAnimBP.Object->GetAnimBlueprintGeneratedClass() );
		return;
	}
	UE_LOG( LogTemp, Error, TEXT( "Unable to load animation blueprint" ) );
}

void UVRMotionControllerComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( PhysicsHandle && m_skeletalMesh )
	{
		const FVector holdLocation = GetGrabSocketLocation();
		PhysicsHandle->SetTargetLocationAndRotation( holdLocation, FRotator::ZeroRotator );
	}

	if( HeldBallComponent && m_skeletalMesh )
	{
		HeldBallComponent->SetWorldLocation( GetGrabSocketLocation() );
	}

	if( !HeldBall )
	{
		SearchForTarget();
	}

	if( TargetBall && !HeldBall )
	{
		DrawDebugLine( GetWorld(), GetGrabSocketLocation(), TargetBallComponent->GetComponentLocation(), FColor::Yellow, false, 0.1, 0, 1 );
	}
}

void UVRMotionControllerComponent::SearchForTarget()
{
	bool foundGrabbable = false;

	// Oof... there must be a better way of doing this
	const FQuat quat = GetSocketQuaternion( TEXT( "grab_socket" ) );
	FVector forward = quat.GetRightVector();
	if( HandSide == EVRHandSide::Right )
	{
		forward *= -1;
	}

	const FVector startLocation = GetGrabSocketLocation();;
	const FVector endLocation = ( ( forward * GrabMaxDistance ) + startLocation );

	FHitResult outHit;
	FCollisionQueryParams collisionParams;
	if( GetWorld()->LineTraceSingleByChannel( outHit, startLocation, endLocation, ECC_Visibility, collisionParams ) )
	{
		if( outHit.bBlockingHit )
		{
			if( ABall* ball = Cast< ABall >( outHit.Actor.Get() ) )
			{
				if( TargetBall != ball )
				{
					// Swap targets
					ClearTargetBall();
					SetNewTargetBall( ball, outHit.Component.Get() );
				}
				foundGrabbable = true;
			}
		}
	}

	if( !foundGrabbable && TargetBall )
	{
		// No longer targeting current ball
		ClearTargetBall();
	}
}

void UVRMotionControllerComponent::SetNewTargetBall( ABall* ball, UPrimitiveComponent* ballComponent )
{
	if( ball && ballComponent )
	{
		TargetBall = ball;
		TargetBallComponent = ballComponent;

		TargetBall->SetInGrabRange( true );
	}
}

void UVRMotionControllerComponent::ClearTargetBall()
{
	if( TargetBall )
	{
		TargetBall->SetInGrabRange( false );
	}
	TargetBall = NULL;
	TargetBallComponent = NULL;
}

void UVRMotionControllerComponent::GrabTargetBall( USceneComponent* parentComponent )
{
	if( TargetBall && TargetBallComponent )
	{
		HeldBall = TargetBall;
		HeldBallComponent = TargetBallComponent;

		HeldBall->Grab();

		ClearTargetBall();

		auto rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		HeldBallComponent->AttachToComponent( m_skeletalMesh, rules, NAME_None );
		HeldBallComponent->SetCollisionResponseToChannel( ECC_Pawn, ECR_Ignore );
		HeldBallComponent->SetSimulatePhysics( false );
	}
}

void UVRMotionControllerComponent::ReleaseHeldBall()
{
	if( HeldBall && HeldBallComponent )
	{
		auto rules = FDetachmentTransformRules::KeepWorldTransform;
		HeldBallComponent->DetachFromComponent( rules );
		HeldBallComponent->SetCollisionResponseToChannel( ECC_Pawn, ECR_Block );
		HeldBallComponent->SetSimulatePhysics( true );

		const FVector offset = PhysicsHandle->TargetTransform.GetTranslation() - PhysicsHandle->CurrentTransform.GetTranslation();
		const FVector velocity = offset * ThrowStrengthMultiplier;
		HeldBallComponent->SetPhysicsLinearVelocity( velocity );

		HeldBall->Release();

		HeldBall = NULL;
		HeldBallComponent = NULL;
	}
}

void UVRMotionControllerComponent::SetGrabAmount( float amount )
{
	m_grabAmount = amount;
	if( m_animBPInstance )
	{
		m_animBPInstance->SetGrabAmount( amount );
	}
}

FVector UVRMotionControllerComponent::GetGrabSocketLocation() const
{
	return m_skeletalMesh ? m_skeletalMesh->GetSocketLocation( TEXT( "grab_socket" ) ) : FVector::ZeroVector;
}
