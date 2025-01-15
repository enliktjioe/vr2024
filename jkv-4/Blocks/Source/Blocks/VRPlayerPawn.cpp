#include "VRPlayerPawn.h"
#include "Base/BlockBaseActor.h"


#include "Components/InputComponent.h"

AVRPlayerPawn::AVRPlayerPawn()
{
    // Enable ticking for the Pawn
    PrimaryActorTick.bCanEverTick = true;

    // Initialize variables
    HeldBlock = nullptr;


    // Create the Child Actor Component
   VRPawnBlueprint = CreateDefaultSubobject<UChildActorComponent>(TEXT("VRPawnBlueprint"));

    // Attach it to the root component of VRPlayerPawn
    VRPawnBlueprint->SetupAttachment(RootComponent);

   // Set the Child Actor Class to your existing VRPawn Blueprint
 VRPawnBlueprint->SetChildActorClass(StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Game/VRTemplate/Blueprints/VRPawn.VRPawn'")));

}

void AVRPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);


}

