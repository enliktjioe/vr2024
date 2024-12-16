/* ©CGVR 2021. Author: Andre Muehlenbrock
*
* A 20x20 clamping block.
*/

#pragma once

#include "CoreMinimal.h"
#include "../Base/BlockBaseActor.h"
#include "Block20x20Actor.generated.h"

UCLASS()
class BLOCKS_API ABlock20x20Actor : public ABlockBaseActor
{
	GENERATED_BODY()

	ABlock20x20Actor() {
		// Finds the mesh of this block and set it:
		static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockMesh(TEXT("StaticMesh'/Game/Blocks/Meshes/Block20x20.Block20x20'"));
		if (BlockMesh.Succeeded()) {
			BlockBaseComponent->SetStaticMesh(BlockMesh.Object);
		}

		VoxelVolume& vVolume = BlockBaseComponent->voxelVolume;

		for (size_t x = 0; x < 20; ++x) {
			for (size_t y = 0; y < 20; ++y) {
				// The lower 20x20 voxels for the female docking points:
				vVolume.Add(x, y, 0, Female);

				// The upper 20x20 voxels for the male docking points:
				vVolume.Add(x, y, 1, Male);
			}
		}
	}
};
