
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"

#include "Actor_Assignment03.h"

// Sets default values
AActor_Assignment03::AActor_Assignment03()
{
    // This makes the actor call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    // Create and attach the static mesh component.
    // MeshComponent will hold and display whatever mesh we assign at runtime.
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Initialize default threshold values.
    LODThreshold1 = 2000000.0f;
    LODThreshold2 = 1000000.0f;

    // Initially, we have no "current" mesh set.
    CurrentMesh = nullptr;
}

// Called when the game starts or when spawned
void AActor_Assignment03::BeginPlay()
{
    Super::BeginPlay();

    // Before we start, let's cache the viewport size so we don't have to get it every frame.
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(CachedViewportSize);
    }

    // Set initial LOD mesh to LODMesh1 if available.
    if (LODMesh1)
    {
        MeshComponent->SetStaticMesh(LODMesh1);
        CurrentMesh = LODMesh1;
    }
}

// Called every frame
void AActor_Assignment03::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Every frame, we update the LOD based on the actor's size on the screen.
    UpdateLOD();
}

void AActor_Assignment03::UpdateLOD()
{
    // If we don't have all the meshes or a MeshComponent, we can't do LOD switching.
    if (!MeshComponent || !LODMesh1 || !LODMesh2 || !LODMesh3) return;

    // Get the actor's bounds in world space.
    // true = consider all components, so we get a correct bounding box.
    FVector BoundsOrigin, BoundsExtent;
    GetActorBounds(true, BoundsOrigin, BoundsExtent);

    // Get the player controller to project world points onto the screen.
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController) return;

    // We will project two opposite corners of the bounding box onto the screen:
    // One corner is (Origin - Extent), the other is (Origin + Extent).
    // This gives us a rough idea of how large the actor appears on the screen.
    FVector2D ScreenPosition1, ScreenPosition2;
    bool bProjected1 = PlayerController->ProjectWorldLocationToScreen(BoundsOrigin - BoundsExtent, ScreenPosition1);
    bool bProjected2 = PlayerController->ProjectWorldLocationToScreen(BoundsOrigin + BoundsExtent, ScreenPosition2);

    // If we fail to project either point (object might be behind camera or no valid controller),
    // just return.
    if (!bProjected1 || !bProjected2) return;

    // Clamp the coordinates so they don't go outside the viewport range.
    ScreenPosition1 = ScreenPosition1.ClampAxes(0, CachedViewportSize.X);
    ScreenPosition2 = ScreenPosition2.ClampAxes(0, CachedViewportSize.Y);

    // Check if the object is off-screen:
    // If either point is at the extreme edges (like 0 or max) this might indicate off-screen.
    // (This is a simplistic check; you can refine as needed.)
    if (FMath::IsNearlyZero(ScreenPosition1.X) || FMath::IsNearlyZero(ScreenPosition1.Y) ||
        FMath::IsNearlyZero(ScreenPosition2.X) || FMath::IsNearlyZero(ScreenPosition2.Y) ||
        ScreenPosition1.X == CachedViewportSize.X || ScreenPosition1.Y == CachedViewportSize.Y ||
        ScreenPosition2.X == CachedViewportSize.X || ScreenPosition2.Y == CachedViewportSize.Y)
    {
        // Off-screen, no LOD update needed.
        return;
    }

    // Compute the screen area of the bounding box.
    float Width = FMath::Abs(ScreenPosition2.X - ScreenPosition1.X);
    float Height = FMath::Abs(ScreenPosition2.Y - ScreenPosition1.Y);
    float ScreenArea = Width * Height;

    // Debug message to see the values in the output log.
    UE_LOG(LogTemp, Warning, TEXT("ScreenArea: %f, LODThreshold1: %f, LODThreshold2: %f"), ScreenArea, LODThreshold1, LODThreshold2);

    // Decide which LOD mesh to use based on ScreenArea:
    // If ScreenArea > LODThreshold1 -> High-detail (LODMesh1)
    // Else if ScreenArea > LODThreshold2 -> Medium-detail (LODMesh2)
    // Else -> Low-detail (LODMesh3)
    UStaticMesh* DesiredMesh = nullptr;
    if (ScreenArea > LODThreshold1)
    {
        DesiredMesh = LODMesh1;
    }
    else if (ScreenArea > LODThreshold2)
    {
        DesiredMesh = LODMesh2;
    }
    else
    {
        DesiredMesh = LODMesh3;
    }

    // Only change the mesh if it's different from the currently applied one.
    if (DesiredMesh && DesiredMesh != CurrentMesh)
    {
        MeshComponent->SetStaticMesh(DesiredMesh);
        CurrentMesh = DesiredMesh;
    }
}

