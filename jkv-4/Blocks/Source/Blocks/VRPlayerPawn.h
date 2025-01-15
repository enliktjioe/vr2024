#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPlayerPawn.generated.h"

UCLASS()
class BLOCKS_API AVRPlayerPawn : public APawn
{
    GENERATED_BODY()

public:
    // Constructor
    AVRPlayerPawn();

    // Input bindings
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    // Held block reference
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Block")
    class ABlockBaseActor* HeldBlock;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VR")
    UChildActorComponent* VRPawnBlueprint;

};
