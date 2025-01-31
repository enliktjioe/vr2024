# Introduction to Digital Media - VR Project

This repository is for VR project of Introduction to Digital Media course from University of Bremen. 

The challenges are as below:

1. Setting up a scene in the Unreal Engine that works in VR
2. Modifying the initial VR scene by adding a few assets to make it look better
3. Implementing a locomotion metaphor other than teleporting

You will find guides and tutorials for the Unreal engine e.g. in the Links section of the VR course: https://cgvr.informatik.uni-bremen.de/teaching/vr/


# Video Demonstration

(via YouTube, click the image to open the link)

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/mCEFvOQ1nWQ/0.jpg)](https://www.youtube.com/watch?v=mCEFvOQ1nWQ)

# Changelog
## IMC_Default

Deleted old movement part:

![](img/IA_Move.png)

New IA_SnapTurn:

![](img/2025-01-31-21-13-49.png)

Updated IMC_Default:

![](img/IMC_Default.png)

## VR_Pawn Blueprint

**Set the Positon Rotation and Height of the Player Capsule**

![](img/2025-01-31-21-19-58.png)

**Primary Inputs for Thumbstick Locomotion**

![](img/2025-01-31-21-20-50.png)

**NEW Event Tick to Update Capsule Height of VR Pawn**

![](img/2025-01-31-21-22-04.png)


**Sets User Specific Max Walk Speed**

![](img/2025-01-31-21-22-35.png)

## Collision Preset

Added new collision preset to prevent `grabable object` messing up the collision with our VR Pawn

![](img/2025-01-31-21-24-06.png)

![](img/2025-01-31-21-25-04.png)

And then change all Collision Presets for these two objects: `SmallCube` and `Pistol` to new preset `InteractableObject`

For `Projectile`, make it custom, where `InteractableObject` became `Overlap`

![](img/2025-01-31-21-27-43.png)

# References:

- https://www.youtube.com/watch?v=KTPx8PeKyQM

# Misc

- Using `Paste Image` extension in VSCode ([source](https://www.janmeppe.com/blog/paste-image/))

![](img/2025-01-31-19-49-55.png)