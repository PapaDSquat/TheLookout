// By Adamo Maiorano (2021)

#include "PlayerHandAnimInstance.h"

UPlayerHandAnimInstance::UPlayerHandAnimInstance( const FObjectInitializer& ObjectInitializer )
{
	GrabAmount = 0.f;
}

void UPlayerHandAnimInstance::SetGrabAmount( float value )
{
	GrabAmount = value;
}
