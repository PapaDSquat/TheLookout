// By Adamo Maiorano (2021)

#include "Ball.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ABall::SetInGrabRange( bool inRange )
{
	if( inRange )
		++m_inGrabRangeCount;
	else
		--m_inGrabRangeCount;

	check( m_inGrabRangeCount >= 0 );

	Execute_OnInGrabRangeChange( this, InGrabRange() );
	return true;
}

bool ABall::Grab()
{
	if( !IsGrabbed )
	{
		IsGrabbed = true;
		Execute_OnPlayerGrab( this );
		return true;
	}
	return false;
}

bool ABall::Release()
{
	if( IsGrabbed )
	{
		IsGrabbed = false;
		Execute_OnPlayerRelease( this );
		return true;
	}
	return false;
}

bool ABall::InGrabRange() const
{
	return m_inGrabRangeCount > 0;
}
