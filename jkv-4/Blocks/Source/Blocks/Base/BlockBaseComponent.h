/* ©CGVR 2021. Author: Andre Muehlenbrock 
* 
* An instance of a BlockBaseComponent describes a single clamping block in the world and inherits from the UStaticMeshComponent.
* Such a UStaticMeshComponent inherits also from the known USceneComponent - this means, that such a block can be positioned,
* rotated and scales relative to the actor. A BlockBaseComponent needs to be attached to a BlockBaseActor to be visible in the
* level (a manual attachment of this component to the BlockBaseActor is NOT needed and already be done in the constructor of the
* BlockBaseActor and it's merging method!).
*
* This file contains some helper enums and structs (like VoxelVolume, VoxelType, BlockTransform and BlockRotation).
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "Math/IntVector.h"

#include <iostream>
#include "DrawDebugHelpers.h"

#include "BlockBaseComponent.generated.h"

/* Defines how a component can be rotated relative to the actor. In this case, a rotation is only possible at the z-Axis (Yaw) 
* and only in steps of 90 degree.
*/

UENUM()
enum BlockRotation {
	Default		UMETA(DisplayName = "Default"),
	Rotate90	UMETA(DisplayName = "Rotate90"),
	Rotate180	UMETA(DisplayName = "Rotate180"),
	Rotate270	UMETA(DisplayName = "Rotate270")
};

/* Defines a transformation of a BlockBaseComponent (in the space of voxels). 
*/

USTRUCT(Blueprintable)
struct FBlockTransform {
	GENERATED_BODY()

	/* Translation in x direction */
	int x;

	/* Translation in y direction */
	int y;
 
	/* Translation in z direction */
	int z;

	/* Rotation */
	TEnumAsByte<BlockRotation> rotation;

	/* Initialize an identity transformation */
	FBlockTransform() : x(0), y(0), z(0), rotation(Default) {};

	/* Initialize a transformation with the given parameters */
	FBlockTransform(int pX, int pY, int pZ, BlockRotation pRotation) : x(pX), y(pY), z(pZ), rotation(pRotation) {};

	/* Initialize a transformation based on the given FTransform (finds the nearest quantized transformation) */
	FBlockTransform(FTransform transform) {
		FVector v2D = transform.GetRotation().RotateVector(FVector(1,0,0));		
		v2D.Z = 0;
		v2D.Normalize();
		float yawRaw = atan2(v2D.Y, v2D.X) * 180 / 3.14159f;
		
		// Rotation should only be made in yaw (in local space of the actor):
		float yaw = FMath::Fmod(yawRaw + 360, 360);

		rotation = Default;
		// Assign the corrent quantized rotation:
		if (yaw >= 45 && yaw < 135)
			rotation = Rotate90;
		else if (yaw >= 135 && yaw < 225)
			rotation = Rotate180;
		else if (yaw >= 225 && yaw < 315)
			rotation = Rotate270;

		FVector location = transform.GetLocation();

		x = int(std::round(location.X));
		y = int(std::round(location.Y));
		z = int(std::round(location.Z * 2));		
	}

	/* Transforms this BlockTransform to a FTransform */
	FTransform ToFTransform() {
		return FTransform(FRotator(0, rotationAsDegree(), 0), FVector(x, y, z / 2.f));
	}

	/* Returns the rotation (Yaw) in degrees */
	int rotationAsDegree() {
		int yaw = 0;

		if (rotation == Rotate90)
			yaw = 90;
		else if (rotation == Rotate180)
			yaw = 180;
		else if (rotation == Rotate270)
			yaw = 270;

		return yaw;
	}
};

/* Defines VoxelTypes */
enum VoxelType {
	Free,		// Free voxel.
	Blocking,	// Blocked voxel.
	Female,		// Female voxel - male voxels can be attached below this voxel.
	Male		// Male voxel - female voxels can be attached above this voxel.
};

/* A volume which saves mutliple voxels.
* A voxel in this volume has a size of 1cm x 1cm x 0.5cm */
struct VoxelVolume {
	/* Maps a coordinate to a voxel */
	TMap<FIntVector, VoxelType> voxels;

	/* Insert a voxel type at the given position */
	void Add(int x, int y, int z, VoxelType type) {
		if (type == Free) {
			voxels.Remove(FIntVector(x, y, z));
			return;
		}
		voxels.Add(FIntVector(x, y, z), type);
	}

	/* Add all voxels of the given voxel volume and inserts it in this volume with the given transformation applied */
	void Add(VoxelVolume vol, FBlockTransform transform) {
		for (auto& element : vol.voxels)
		{
			FIntVector newTransform = TransformVector(element.Key, transform);
			voxels.Add(newTransform, element.Value);
		}
	}

	/* Returns the voxel type at the given location */
	VoxelType Get(FIntVector vec) {
		if (voxels.Contains(vec)) {
			return voxels[vec];
		}

		return VoxelType::Free;
	}
	
	/* Applies a transformation to this voxel volume (and transforms all voxels) */
	VoxelVolume TransformTo(FBlockTransform transform) {
		VoxelVolume result;

		for (auto& element : voxels)
		{
			result.voxels.Add(TransformVector(element.Key, transform), element.Value);
		}

		return result;
	}

	/* Applies the given transformation to the given location v */
	static FIntVector TransformVector(FIntVector v, FBlockTransform transform) {
		int mX = v.X;
		int mY = v.Y;
		int mZ = v.Z;

		if (transform.rotation == Rotate90) {
			mX = -v.Y;
			mY = v.X;
		}
		else if (transform.rotation == Rotate180) {
			mX = -v.X;
			mY = -v.Y;
		}
		else if (transform.rotation == Rotate270) {
			mX = v.Y;
			mY = -v.X;
		}

		mX += transform.x;
		mY += transform.y;
		mZ += transform.z;

		return FIntVector(mX, mY, mZ);
	}

	/* Returns the center of the bounding box of this volume */
	FVector getCenter(){
		FVector min(10000000, 10000000, 10000000);
		FVector max(-10000000, -10000000, -10000000);

		for (auto& Elem : voxels) {
			FIntVector e = Elem.Key;

			if (e.X > max.X) max.X = e.X;
			if (e.Y > max.Y) max.Y = e.Y;
			if (e.Z > max.Z) max.Z = e.Z;
			if (e.X < min.X) min.X = e.X;
			if (e.Y < min.Y) min.Y = e.Y;
			if (e.Z < min.Z) min.Z = e.Z;
		}

		return (max + min) / 2;
	}

	/* Prints all occupied positions of this volume */
	void print() {
		UE_LOG(LogBlueprint, Warning, TEXT("VOLUME"));
		for (auto& Elem : voxels) {
			UE_LOG(LogBlueprint, Warning, TEXT(" \t%i, %i, %i"), Elem.Key.X, Elem.Key.Y, Elem.Key.Z);
		}
	}

	/* Debug draws this voxel volume. THIS METHOD IS SLOW AND NOT APPROPIATE FOR HIGHLIGHTING! */
	void debugDraw(UWorld* world, FTransform transform, float thickness = 0.05f) {
		for (auto& Elem : voxels) {
			FVector vec = FVector(Elem.Key.X, Elem.Key.Y, Elem.Key.Z / 2.f);
			FVector absolutePos = transform.TransformPosition(vec);

			FColor color;
			if (Elem.Value == Male)
				color = FColor::Blue;
			else if (Elem.Value == Female)
				color = FColor::Magenta;

			if (Elem.Value != Free)
				DrawDebugBox(world, absolutePos, FVector(0.5f, 0.5f, 0.25f), transform.GetRotation(), color, false, 0.f, ECC_WorldStatic, thickness);
		}
	}
};

UCLASS(Blueprintable)
class BLOCKS_API UBlockBaseComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	/* The voxel volume of this single block */
	VoxelVolume voxelVolume;
	
};
