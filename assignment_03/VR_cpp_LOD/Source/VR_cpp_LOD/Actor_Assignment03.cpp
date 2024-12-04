// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Assignment03.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"


// Sets default values
AActor_Assignment03::AActor_Assignment03()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create and attach the static mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Initialize default values
    LODThreshold1 = 2000000.0f;
    LODThreshold2 = 1000000.0f;

}

// Called when the game starts or when spawned
void AActor_Assignment03::BeginPlay()
{
	Super::BeginPlay();

    // Set initial LOD
    if (LODMesh1)
    {
        MeshComponent->SetStaticMesh(LODMesh1);
    }
}

// Called every frame
void AActor_Assignment03::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateLOD();
    
    // // Check if the actor has moved since the last frame
    // static FVector LastLocation = GetActorLocation();
    // FVector CurrentLocation = GetActorLocation();

    // if (!CurrentLocation.Equals(LastLocation, KINDA_SMALL_NUMBER))
    // {
    //      // Update LOD if there is significant movement
    //      UpdateLOD();
    //      LastLocation = CurrentLocation;
    // }
}

void AActor_Assignment03::UpdateLOD()
{
    // Only for assigned Meshes
    if (!MeshComponent || !LODMesh1 || !LODMesh2 || !LODMesh3) return;

    // Get the bounds of the mesh
    FVector Origin, Extent;
    MeshComponent->GetLocalBounds(Origin, Extent); // Bounds in world space

    // Project the world location of the bounds to screen space
    FVector2D ScreenPosition1, ScreenPosition2;
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController) return;

    PlayerController->ProjectWorldLocationToScreen(Origin - Extent, ScreenPosition1);
    PlayerController->ProjectWorldLocationToScreen(Origin + Extent, ScreenPosition2);

    // Clamp the coordinates within the screen viewport
    FVector2D ViewportSize;
    GEngine->GameViewport->GetViewportSize(ViewportSize);

    ScreenPosition1 = ScreenPosition1.ClampAxes(0, ViewportSize.X);
    ScreenPosition2 = ScreenPosition2.ClampAxes(0, ViewportSize.Y);

    // Object Outside the Screen: Skip LOD updates if the object is completely out of view.
    if (ScreenPosition1.X >= ViewportSize.X || ScreenPosition2.X <= 0 ||
        ScreenPosition1.Y >= ViewportSize.Y || ScreenPosition2.Y <= 0)
    {
        return; // Object is outside the screen
    }

    // Calculate the screen-space size (rough area)
    float ScreenArea = FMath::Abs((ScreenPosition2.X - ScreenPosition1.X) * (ScreenPosition2.Y - ScreenPosition1.Y));

    // Debugging message in OutputLog UE
    UE_LOG(LogTemp, Warning, TEXT("ScreenArea: %f, LODThreshold1: %f, LODThreshold2: %f"), ScreenArea, LODThreshold1, LODThreshold2);

    // Switch LOD based on thresholds
    if (ScreenArea > LODThreshold1)
    {
        MeshComponent->SetStaticMesh(LODMesh1); // High-detail mesh
    }
    else if (ScreenArea > LODThreshold2)
    {
        MeshComponent->SetStaticMesh(LODMesh2); // Medium-detail mesh
    }
    else
    {
        MeshComponent->SetStaticMesh(LODMesh3); // Low-detail mesh
    }
     
    // // Get the bounds of the mesh
    // FVector Origin, Extent;
    // MeshComponent->GetLocalBounds(Origin, Extent);

    // FVector2D ScreenPosition;
    // APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    // if (PlayerController && PlayerController->ProjectWorldLocationToScreen(Origin, ScreenPosition))
    // {
    //     // FVector ScreenBounds = Extent * PlayerController->GetFOVAngle() / 90.0f; // Normalize to VR FOV
    //     // float ScreenArea = ScreenBounds.SizeSquared(); // Approximation of screen size
        
    //     FVector2D ScreenSize;
    //     UGameplayStatics::GetPlayerController(GetWorld(), 0)->ProjectWorldLocationToScreen(GetActorLocation(), ScreenSize);

    //     float ScreenArea = Extent.SizeSquared(); // Approximate screen area
    //     FVector2D ViewportSize;
    //     GEngine->GameViewport->GetViewportSize(ViewportSize);

    //     // Debugging message in OutputLog UE
    //     UE_LOG(LogTemp, Warning, TEXT("ScreenArea: %f, LODThreshold1: %f"), ScreenArea, LODThreshold1);

    //     if (ScreenArea > LODThreshold1)
    //     {
    //         MeshComponent->SetStaticMesh(LODMesh1); // High-detail mesh
    //     }
    //     else if (ScreenArea > LODThreshold2)
    //     {
    //         MeshComponent->SetStaticMesh(LODMesh2); // Medium-detail mesh
    //     }
    //     else
    //     {
    //         MeshComponent->SetStaticMesh(LODMesh3); // Low-detail mesh
    //     }
    // }
}