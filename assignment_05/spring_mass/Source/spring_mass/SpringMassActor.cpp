// Fill out your copyright notice in the Description page of Project Settings.

#include "SpringMassActor.h"
#include "spring_mass.h"


// Sets default values
ASpringMassActor::ASpringMassActor() : m_deltaTimeRemaining(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// just for better handling
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;

	// mesh for visualization
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));

	// create mesh and spring system
	initSpringSystem();

	mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpringMassActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpringMassActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// use a fixed timestep
	float step = 1 / 200.0f;
	// time we did not simulate from last step
	DeltaTime += m_deltaTimeRemaining;
	while (DeltaTime >= step) {
		DeltaTime -= step;

		// calculate force between mass points
		for (Spring& s : springs){
			s.Tick();
		}

		// update positions
		for (MassPoint& m : massPoints) {
			// add gravity
			m.updateGravity();
			m.updateCurPos(step);
		}
	}
	// save remaining time for next tick
	m_deltaTimeRemaining = DeltaTime;

	// update vertices in mesh
	for (MassPoint& m : massPoints) {
		vertices[m.getVertexId()] = m.getCurrPos();
	}
	mesh->UpdateMeshSection(1, vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
}

void ASpringMassActor::initSpringSystem()
{
	TArray<int32> Triangles;

	// Set up vertices and mass points
	for (uint16 x = 0; x < cols; x++) {
		for (uint16 z = 0; z < rows; z++) {
			auto v = FVector(size * x, 0, size * z);
			vertices.Add(v);

			// mass points
			uint32 id = x * rows + z;
			// upper row ist fixed
			bool movable = z != rows - 1;
			// only corners
			//bool movable = z != rows - 1 || (x != 0 && x != cols - 1);
			massPoints.Add(MassPoint(id, movable, v));
		}
	}

	// Set up indices
	for (uint16 x = 0; x < cols-1; x++)
	{
		for (uint16 z = 0; z < rows-1; z++)
		{
			// quad: tri 1
			uint32 offset = x * rows + z;
			Triangles.Add(offset + 0);
			Triangles.Add(offset + 1);
			Triangles.Add(offset + rows);
			// tri 2
			Triangles.Add(offset + 1);
			Triangles.Add(offset + rows+1);
			Triangles.Add(offset + rows);
		}
	}

	// add springs for each vertice v
	//  nw  n  ne
	//      | / 
	//  w   v - e
	//        \
	// sw   s  se
	for (uint16 x = 0; x < cols; x++)
	{
		for (uint16 z = 0; z < rows; z++)
		{
			// current vertice
			uint32 id = x * rows + z;
			FVector v = vertices[id];
			// we are not at the east edge
			if (x < cols - 1) {
				uint32 id_e = (x + 1) * rows + z;
				FVector v_e = vertices[id_e];
				springs.Add(Spring(&massPoints[id], &massPoints[id_e], FVector::Dist(v, v_e)));
				// we are not in the south east corner -> add spring
				if (z > 0) {
					uint32 id_se = (x + 1) * rows + (z - 1);
					FVector v_se = vertices[id_se];
					springs.Add(Spring(&massPoints[id], &massPoints[id_se], FVector::Dist(v, v_se)));
				}
				// we are not in the north east corner -> add spring
				if (z < rows - 1) {
					uint32 id_ne = (x + 1) * rows + (z + 1);
					FVector v_ne = vertices[id_ne];
					springs.Add(Spring(&massPoints[id], &massPoints[id_ne], FVector::Dist(v, v_ne)));
				}
			}

			// we are not at the north edge
			if (z < rows - 1){
				uint32 id_n = x * rows + (z + 1);
				FVector v_n = vertices[id_n];
				springs.Add(Spring(&massPoints[id], &massPoints[id_n], FVector::Dist(v, v_n)));
			}
		}
	}
	
	// instanciate mesh
	mesh->CreateMeshSection(1, vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

void ASpringMassActor::Touch()
{
	// add force to the center
	uint32 id = (cols / 2) * rows + (rows / 2);
	massPoints[id].addForce(FVector(0, 20, 0));
}
