/* ©CGVR 2021. Author: Andre Muehlenbrock
*
* Transfered the blueprint interface for Pickup and Drop objects to make it work with C++ actors and blueprint actors both.
*/

#pragma once

#include "Components/SceneComponent.h"

#include "PickupActorInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UPickupActorInterface : public UInterface
{
    GENERATED_BODY()
};

class IPickupActorInterface
{    
    GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Pickup(USceneComponent* AttachTo);
		
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Drop();

};