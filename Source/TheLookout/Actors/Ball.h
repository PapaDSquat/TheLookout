// By Adamo Maiorano (2021)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GrabbableInterface.h"
#include "Ball.generated.h"

UCLASS()
class THELOOKOUT_API ABall : public AActor, public IGrabbableInterface
{
	GENERATED_BODY()
	
public:	
	ABall();

public:	
	// IGrabbableInterface
	virtual bool SetInGrabRange( bool inRange ) override;
	virtual bool Grab() override;
	virtual bool Release() override;

	UFUNCTION( BlueprintCallable, Category = Grab )
	bool InGrabRange() const;

	UPROPERTY( BlueprintReadOnly, Category = Grab )
	bool IsGrabbed;

private:
	std::atomic< int8 > m_inGrabRangeCount;
};
