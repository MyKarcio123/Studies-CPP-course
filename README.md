Repo includes a project for a college course on C++ programming
My project is a voxel-based, multi-threaded graphics engine written using OpenGL. The engine is integrated with a procedural world generator so that the player can get a glimpse of what everything looks like.
The engine as well as the generator is a preliminary version of alpha 0.1 . It includes the following:
- loading block graphics from a single texture atlas,
- three-dimensional camera,
- generation of only the player-visible walls of models,
- filling chunks on a separate thread with data,
- creating a mesh for the chunks and texturing them on a separate thread,
- generating procedural worlds using perlin noise, making none the same.

Controls:
- with the mouse you can rotate the camera 360 degrees
- with the standard keys w,a,s,d you can move the flying player (also you can fly into the chunks, so you can notice that from the inside they are not generated to save time and space)

In addition, in the dependencies folder I included the external libraries I used:
-FastNoise
-GLFW
-SOIL
-glew
-glm
