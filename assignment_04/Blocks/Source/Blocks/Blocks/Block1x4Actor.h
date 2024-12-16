/* ©CGVR 2021. Author: Andre Muehlenbrock
*
* A 1x4 clamping block.
*/

#pragma once

#include "CoreMinimal.h"
#include "../Base/BlockBaseActor.h"
#include "Block1x4Actor.generated.h"

UCLASS()
class BLOCKS_API ABlock1x4Actor : public ABlockBaseActor
{
	GENERATED_BODY()

	ABlock1x4Actor() {
		// Finds the mesh of this block and set it:
		static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockMesh(TEXT("StaticMesh'/Game/Blocks/Meshes/Block1x4.Block1x4'"));
		if (BlockMesh.Succeeded()) {
			BlockBaseComponent->SetStaticMesh(BlockMesh.Object);
		}

		VoxelVolume& voxelVolume = BlockBaseComponent->voxelVolume;

		// The lower four voxels for the female docking points:
		voxelVolume.Add(0, 0, 0, Female);
		voxelVolume.Add(0, 1, 0, Female);
		voxelVolume.Add(0, 2, 0, Female);
		voxelVolume.Add(0, 3, 0, Female);

		// The upper four voxels for the male docking points:
		voxelVolume.Add(0, 0, 1, Male);
		voxelVolume.Add(0, 1, 1, Male);
		voxelVolume.Add(0, 2, 1, Male);
		voxelVolume.Add(0, 3, 1, Male);
	}
};
