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
    LODThreshold1 = 1000.0f;
    LODThreshold2 = 500.0f;

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

    // Check if the actor has moved since the last frame
    static FVector LastLocation = GetActorLocation();
    FVector CurrentLocation = GetActorLocation();

    if (!CurrentLocation.Equals(LastLocation, KINDA_SMALL_NUMBER))
    {
        // Update LOD if there is significant movement
        UpdateLOD();
        LastLocation = CurrentLocation;
    }
}

void AActor_Assignment03::UpdateLOD()
{
    if (!MeshComponent || !LODMesh1 || !LODMesh2 || !LODMesh3) return;

    // Get the bounds of the mesh
    FVector Origin, BoxExtent;
    MeshComponent->GetLocalBounds(Origin, BoxExtent);

    FVector2D ScreenSize;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->ProjectWorldLocationToScreen(GetActorLocation(), ScreenSize);

    float Area = BoxExtent.SizeSquared(); // Approximate screen area
    FVector2D ViewportSize;
    GEngine->GameViewport->GetViewportSize(ViewportSize);

    // Switch based on thresholds
    if (Area > LODThreshold1)
    {
        MeshComponent->SetStaticMesh(LODMesh1);
    }
    else if (Area > LODThreshold2)
    {
        MeshComponent->SetStaticMesh(LODMesh2);
    }
    else
    {
        MeshComponent->SetStaticMesh(LODMesh3);
    }
}