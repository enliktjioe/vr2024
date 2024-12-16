/* ©CGVR 2021. Author: Andre Muehlenbrock
*
* A 2x2 clamping block.
*/

#pragma once

#include "CoreMinimal.h"
#include "../Base/BlockBaseActor.h"
#include "Block2x2Actor.generated.h"

UCLASS()
class BLOCKS_API ABlock2x2Actor : public ABlockBaseActor
{
	GENERATED_BODY()

	ABlock2x2Actor() {
		// Finds the mesh of this block and set it:
		static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockMesh(TEXT("StaticMesh'/Game/Blocks/Meshes/Block2x2.Block2x2'"));
		if (BlockMesh.Succeeded()) {
			BlockBaseComponent->SetStaticMesh(BlockMesh.Object);
		}

		VoxelVolume& vVolume = BlockBaseComponent->voxelVolume;

		// The lower four voxels for the female docking points:
		vVolume.Add(0, 0, 0, Female);
		vVolume.Add(1, 0, 0, Female);
		vVolume.Add(0, 1, 0, Female);
		vVolume.Add(1, 1, 0, Female);

		// The upper four voxels for the male docking points:
		vVolume.Add(0, 0, 1, Male);
		vVolume.Add(1, 0, 1, Male);
		vVolume.Add(0, 1, 1, Male);
		vVolume.Add(1, 1, 1, Male);
	}
};
