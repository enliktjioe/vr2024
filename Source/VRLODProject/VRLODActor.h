#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Actor_Assignment03.generated.h" // MUST BE THE LAST INCLUDE

UCLASS(Blueprintable)
class VR_CPP_LOD_API AActor_Assignment03 : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AActor_Assignment03();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // The main visible component of this Actor
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LOD")
    UStaticMeshComponent* MeshComponent;

    // Three different LOD (Level Of Detail) meshes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    UStaticMesh* LODMesh1;  // High-detail mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    UStaticMesh* LODMesh2;  // Medium-detail mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    UStaticMesh* LODMesh3;  // Low-detail mesh

    // Threshold values that determine when to switch between the meshes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    float LODThreshold1; // If above this screen area -> use LODMesh1
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    float LODThreshold2; // If above this screen area but below LODThreshold1 -> use LODMesh2
                         // Otherwise -> LODMesh3

private:
    // Updates the LOD based on how big the actor appears on the screen
    void UpdateLOD();

    // Stores the currently applied mesh so we don't reset it unnecessarily
    UStaticMesh* CurrentMesh;

    // Caches the viewport size so we don't query it every frame
    FVector2D CachedViewportSize;
};
