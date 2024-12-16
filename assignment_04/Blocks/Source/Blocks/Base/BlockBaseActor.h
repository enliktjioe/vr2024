/* ©CGVR 2021. Author: Andre Muehlenbrock 
* 
* A BlockBaseActor is a movable and grabbable actor, which can have several BlockBaseComponets.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "BlockBaseComponent.h"
#include "../Interface/PickupActorInterface.h"

#include "BlockBaseActor.generated.h"

UCLASS(Blueprintable)
class BLOCKS_API ABlockBaseActor : public AActor, public IPickupActorInterface
{
	GENERATED_BODY()
	
public:	
	/* Initializes the scene root */
	ABlockBaseActor();

	/* The root component */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Blocks)
		USceneComponent* PrimitiveComponentRoot;

	/* The initial assigned clamping block */
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = Blocks)
		UBlockBaseComponent* BlockBaseComponent;

	/* All block components mapped to their current position relative to this actor */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Blocks)
		TMap<UBlockBaseComponent*, FBlockTransform> blocks;

	/* Stores the overlapping actors temporary, can be used for highlighting */
	TMap<ABlockBaseActor*, UBlockBaseComponent*> overlappingActors;

	/* Called when another component collides roughly with a component of this actor */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* t, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Called when another component collides roughly with a component of this actor */
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* t, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* Called from the BP_MotionController if the player grabs this actor */
	void Pickup_Implementation(USceneComponent* AttachTo) override;

	/* Called from the BP_MotionController if the player releases this actor after grabbing it */
	void Drop_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/* Returns the current voxel volume containing all components (interal data structure to store where other bricks can be attached to) */
	VoxelVolume currentVolume();

	/* Checks whether this actor can be merged to the given actor based on the position and orientation of both */
	bool isMergableTo(ABlockBaseActor* actor);

	/* Returns the heuristly best fitting transformation of this actor relative to the given actor, already "quantized" so that the blocks 
	* fit exactly to each other. However, this functions DOES NOT check whether blocks are overlapping or whether a connection exists -
	* therefore, use the isMergableTo- or mergeTo-functions.
	*
	* Additionally, it's better to transform a smaller BlockBaseActor into a bigger one because the error of rotation based on this heuristic
	* will be lower.
	*/
	FTransform getBlockTransformRelativeTo(ABlockBaseActor* actor);

	/* Returns the dimension of the voxels (e.g. 2x2x2 for the Block2x2Actor. Keep in mind that a voxel has a size of 1x1x0.5). */
	FIntVector GetVoxelDimension();

	/* Returns the volume of a voxel volume. This method already divides the z-axis by 2, so that the result is the real volume in cm^3 */
	float getVoxelDimensionVolume();

	/* Merges all the blocks of one actor to the given actor by copying them and deletes itself afterwards. So don't use a reference to
	* this BlockBaseActor after you merged it to another actor.
	*/
	bool mergeTo(ABlockBaseActor* actor);

	/* Stores whether this actor was removed from the world previously.
	* This is checked in the collision listeners to avoid function executions when this actor was already deleted.
	*/
	bool mergeRemoved = false;
};
