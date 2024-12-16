// ©CGVR 2021. Author: Andre Muehlenbrock 

#include "BlockBaseActor.h"
#include "Kismet/KismetSystemLibrary.h"

ABlockBaseActor::ABlockBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a default object:
	PrimitiveComponentRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	
	BlockBaseComponent = CreateDefaultSubobject<UBlockBaseComponent>("BlockBaseComponent");
	BlockBaseComponent->AttachToComponent(PrimitiveComponentRoot, FAttachmentTransformRules::KeepRelativeTransform);

	// Enable overlap events and register overlap listeners:
	BlockBaseComponent->SetGenerateOverlapEvents(true);
	BlockBaseComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlockBaseActor::OnOverlapBegin);        // set up a notification for when this component overlaps something
	BlockBaseComponent->OnComponentEndOverlap.AddDynamic(this, &ABlockBaseActor::OnOverlapEnd);      // set up a notification for when this component overlaps something
	BlockBaseComponent->SetCollisionProfileName(FName("OverlapAll"));

	// Inserts the BlockBaseComponent at 0,0,0:
	blocks.Add(BlockBaseComponent, FBlockTransform());

	// Sets the root component:
	RootComponent = PrimitiveComponentRoot;
}

// Called when the game starts or when spawned
void ABlockBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlockBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Copies the map of overlapping actors to work with it:
	TMap<ABlockBaseActor*, UBlockBaseComponent*> overlappingCopy = overlappingActors;

	// Check all overlapping actors whether merging is possible with the current location and rotation:
	for (auto& Elem : overlappingCopy) {
		// Merge the smaller BlockBaseActor to the bigger one (because so the BlockTransformation will be found for the smaller block relative 
		// to the bigger one):
		if (getVoxelDimensionVolume() >= Elem.Key->getVoxelDimensionVolume()) {
			// Debug draw the elements:
			// currentVolume().debugDraw(GetWorld(), GetTransform(), 0.15f);
			// Elem.Key->currentVolume().debugDraw(GetWorld(), Elem.Key->getBlockTransformRelativeTo(this, Elem.Value) * GetTransform(), 0.15f);

			if (Elem.Key->mergeTo(this)) {
				// If you want to do something when merging is performed, place it here.
			}

		}
	}


}

void ABlockBaseActor::OnOverlapBegin(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && Comp)
	{
		if (OtherActor->IsA(ABlockBaseActor::StaticClass()) && Comp->IsA(UBlockBaseComponent::StaticClass())) {
			//UKismetSystemLibrary::PrintString(GetWorld(), "COLLISION STARTED!");
			overlappingActors.Add((ABlockBaseActor*)OtherActor, (UBlockBaseComponent*) Comp);
		}
	}
}

void ABlockBaseActor::OnOverlapEnd(UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp && Comp)
	{
		if (OtherActor->IsA(ABlockBaseActor::StaticClass()) && Comp->IsA(UBlockBaseComponent::StaticClass())) {
			//UKismetSystemLibrary::PrintString(GetWorld(), "COLLISION ENDED!");
			overlappingActors.Remove((ABlockBaseActor*)OtherActor);
		}
	}
}

void ABlockBaseActor::Pickup_Implementation(USceneComponent* AttachTo)
{
	K2_GetRootComponent()->K2_AttachToComponent(AttachTo, FName("None"), EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
}


void ABlockBaseActor::Drop_Implementation()
{
	K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
}

VoxelVolume ABlockBaseActor::currentVolume() {
	VoxelVolume result;

	for (auto& Elem : blocks)
	{
		// Adds the voxels into the result volume and applies the block transformation:
		result.Add(Elem.Key->voxelVolume, Elem.Value);
	}

	return result;
}

bool ABlockBaseActor::isMergableTo(ABlockBaseActor* actor) {
	VoxelVolume targetVolume = actor->currentVolume();
	VoxelVolume sourceVolume = currentVolume().TransformTo(FBlockTransform(getBlockTransformRelativeTo(actor)));

	// Variable to count the direct connections of male and female blocks:
	int connections = 0;

	for (auto& Elem : sourceVolume.voxels){
		FIntVector vec = Elem.Key;
		VoxelType type = Elem.Value;

		// For every un-"free" type, the slot has to be free in the target actor:
		if (targetVolume.Get(vec) != Free) {
			return false;
		}

		// If blocking, the lower part is not allowed to be male:
		if (type == Blocking) {
			VoxelType lowerTarget = targetVolume.Get(vec + FIntVector(0, 0, -1));
			if (lowerTarget == Male) {
				return false;
			}
		}

		// If female, the lower part must be free or male:
		if (type == Female) {
			FIntVector iVec = vec + FIntVector(0, 0, -1);
			VoxelType lowerTarget = targetVolume.Get(iVec);

			if (lowerTarget != Male && lowerTarget != Free) {
				return false;
			}

			if (lowerTarget == Male)
				++connections;
		}

		// If male, the upper part must be free or male:
		if (type == Male) {
			FIntVector iVec = vec + FIntVector(0, 0, 1);
			VoxelType upperTarget = targetVolume.Get(iVec);

			if (upperTarget != Female && upperTarget != Free) {
				return false;
			}

			if (upperTarget == Female)
				++connections;
		}
	}

	// Connections of male and female voxel is needed, otherwise return false:
	if (connections == 0) {
		return false;
	}

	return true;
}

FTransform ABlockBaseActor::getBlockTransformRelativeTo(ABlockBaseActor* actor) {
	// Calculate the delta transformation from actor to this:
	FTransform deltaTransform = GetActorTransform() * actor->GetActorTransform().Inverse();
	FVector deltaV = deltaTransform.GetLocation();
	FQuat rotation = deltaTransform.GetRotation();

	// We only want the 2D-rotation:
	FVector v2D = rotation.RotateVector(FVector(1, 0, 0));
	v2D.Z = 0;
	v2D.Normalize();
	float yawRaw = atan2(v2D.Y, v2D.X) * 180 / 3.14159f;


	// Rotation should only be made in yaw (in local space of the actor):
	float yaw = FMath::Fmod(yawRaw + 360, 360);

	// Quantized rotation (only yaw rotation with 0, 90, 180 or 270 degree is allowed):
	FRotator quantRotation(0, 0, 0);

	// Assign the corrent quantized rotation:
	if (yaw >= 45 && yaw < 135)
		quantRotation = FRotator(0, 90, 0);
	else if (yaw >= 135 && yaw < 225)
		quantRotation = FRotator(0, 180, 0);
	else if (yaw >= 225 && yaw < 315)
		quantRotation = FRotator(0, 270, 0);

	// Because (0,0,0) is not the midpoint but the corner of the block, we have to center
	// the block to get a good rotation:
	FIntVector iVec = GetVoxelDimension();
	FVector rotationOffset(iVec.X / 2.f, iVec.Y / 2.f, iVec.Z / 4.f);

	// Center block and rotate (only 0, 90, 180 or 270):
	FTransform rotationTransform = (FTransform(-rotationOffset) * FTransform(quantRotation));

	// The final transformation is the relative location plus the "normal" rotated halfBlockSize:
	FTransform locationTransform = FTransform(deltaTransform.GetLocation() + rotation.RotateVector(rotationOffset));

	// Apply rotation and offset to the new transform:
	FTransform newTransform = rotationTransform * locationTransform;

	// Quantize also vector
	FVector nTVector = newTransform.GetLocation();

	nTVector.X = std::round(nTVector.X);
	nTVector.Y = std::round(nTVector.Y);
	nTVector.Z = std::round(nTVector.Z);

	// Return merged quanized transformation:
	return FTransform(newTransform.GetRotation(), nTVector);
}

FIntVector ABlockBaseActor::GetVoxelDimension() {
	VoxelVolume volume = currentVolume();

	FIntVector min(1000000, 1000000, 1000000);
	FIntVector max(-1000000, -1000000, -1000000);

	for (auto& Elem : volume.voxels) {
		FIntVector e = Elem.Key;

		if (e.X > max.X) max.X = e.X;
		if (e.Y > max.Y) max.Y = e.Y;
		if (e.Z > max.Z) max.Z = e.Z;
		if (e.X < min.X) min.X = e.X;
		if (e.Y < min.Y) min.Y = e.Y;
		if (e.Z < min.Z) min.Z = e.Z;
	}

	// If min is bigger than max, no voxel was in this volume:
	if (min.X > max.Z)
		return FIntVector(0, 0, 0);

	return (max - min) + FIntVector(1, 1, 1);
}

float ABlockBaseActor::getVoxelDimensionVolume() {
	FIntVector dimension = GetVoxelDimension();

	return dimension.X * dimension.Y * (dimension.Z / 2.f);
}

bool ABlockBaseActor::mergeTo(ABlockBaseActor* actor) {
	if (this == actor || !isMergableTo(actor))
		return false;

	if (mergeRemoved || actor->mergeRemoved)
		return false;

	//Mark already here as removed
	mergeRemoved = true;

	FTransform actorToThisTransform = getBlockTransformRelativeTo(actor);

	// Copy all BlockBaseComponents:
	for (auto& Elem : blocks) {
		// Remove listeners:
		//Elem.Key->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Elem.Key->OnComponentBeginOverlap.RemoveAll(this);
		Elem.Key->OnComponentEndOverlap.RemoveAll(this);

		// Create new BlockBaseComponent:
		UBlockBaseComponent* copy = NewObject<UBlockBaseComponent>(actor);
		copy->SetStaticMesh(Elem.Key->GetStaticMesh());
		copy->AttachToComponent(actor->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		FTransform elemTransf = blocks[Elem.Key].ToFTransform();
		FTransform blockTransform = blocks[Elem.Key].ToFTransform() * actorToThisTransform;

		copy->SetRelativeTransform(blockTransform);
		copy->RegisterComponent();
		copy->voxelVolume = Elem.Key->voxelVolume;
		copy->OnComponentBeginOverlap.AddDynamic(actor, &ABlockBaseActor::OnOverlapBegin);
		copy->OnComponentEndOverlap.AddDynamic(actor, &ABlockBaseActor::OnOverlapEnd);
		copy->SetCollisionProfileName(FName("OverlapAll"));

		FVector v = blockTransform.GetLocation();

		actor->blocks.Add(copy, FBlockTransform(blockTransform));
	}

	// Destroy the actor:
	GetWorld()->DestroyActor(this);

	return true;
}