// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Actor_Assignment03.generated.h"

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

    // Root mesh component
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LOD")
        UStaticMeshComponent* MeshComponent;

    // LOD Meshes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
        UStaticMesh* LODMesh1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
        UStaticMesh* LODMesh2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
        UStaticMesh* LODMesh3;

    // Thresholds for LOD switching
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
        float LODThreshold1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
        float LODThreshold2;

private:
    void UpdateLOD();

};
