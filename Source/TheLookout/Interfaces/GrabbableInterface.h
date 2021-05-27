// By Adamo Maiorano (2021)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrabbableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGrabbableInterface : public UInterface
{
	GENERATED_BODY()
};

class THELOOKOUT_API IGrabbableInterface
{
	GENERATED_BODY()

public:
	virtual bool SetInGrabRange( bool inRange ) = 0;
	virtual bool Grab() = 0;
	virtual bool Release() = 0;

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = Grab )
	void OnInGrabRangeChange( bool inRange );

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = Grab )
	void OnPlayerGrab();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = Grab )
	void OnPlayerRelease();

};
