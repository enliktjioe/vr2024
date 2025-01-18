// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassPoint.h"
#include "Spring.h"

#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SpringMassActor.generated.h"

UCLASS()
class SPRING_MASS_API ASpringMassActor : public AActor
{
	GENERATED_BODY()

	// vertex data for visualization
	TArray<FVector> vertices;

	// mass-spring system data
	TArray<MassPoint> massPoints;
	TArray<Spring> springs;

	// create mesh and mass-spring system
	void initSpringSystem();

	// time not simulated in last tick, used for fixed delta time updates
	float m_deltaTimeRemaining;

	// shape of the mesh
	uint16 rows = 20;
	uint16 cols = 40;
	float size = 10;

public:	
	// Sets default values for this actor's properties
	ASpringMassActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Export mesh so we can apply a material in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* mesh;

	// Add a force to our system
	UFUNCTION(BlueprintCallable, Category = "Main")
	void Touch();
};
