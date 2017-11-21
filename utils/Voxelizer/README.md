# README #

Voxelization program that can be run via command line. Requires input and output files to be given via command line to execute. For example:
./Voxelizer --input-obj tree.obj --output-path voxeltree.txt
The voxelizer defaults to using a resolution of 64x64x64, but you can change this by adding the argument --resolution [a positive integer]. 

Here's a fancy part: You can view a (currently really shitty) OpenGL output of the voxelized representation by including the argument --visualize [yes|no]. Youre just a floating camera that can use WASD and mouse to look around the scene.

