# GG1-C38-Fast-Fluid-Dynamics-on-the-GPU
From [GPU Gems book 1, part 6, chapter 38.](https://developer.nvidia.com/gpugems/gpugems/part-vi-beyond-triangles/chapter-38-fast-fluid-dynamics-simulation-gpu)
 Mark J. Harris, University of North Carolina at Chapel Hill

## Concepts Implemented
* 2D Stam stable fluids

## Visuals
![Fluids](https://user-images.githubusercontent.com/42471346/189209650-902c2869-6dde-48ab-a108-0a0549eb9ffb.gif)


## Concepts Reimagined
* TBD

## Concepts explored/reimplemented in other repositories
* TBD

## Compilation and installation instructions
The graphics for this engine are handled using OpenGL, and the following instructions are for setting up the code in Windows. 

### Required packages
Compiler used:
* Minimalist GNU for Windows ([MinGW](https://sourceforge.net/projects/mingw/))
  * A GCC Compiler

Libraries required for default program compilation and installed under dependency folder:
* [OpenGL](https://github.com/KhronosGroup/OpenGL-Registry)
* The OpenGL Extension Wrangler Library ([GLEW](https://github.com/nigels-com/glew))
* Simple DirectMedia Layer ([SDL](https://github.com/libsdl-org/SDL))
  * SDL Image

### Instructions
* All dependencies except Assimp are stored within the repository. Once MinGW is installed, make sure you add ~\MinGW\bin to the PATH environment variable using these [instructions](http://www.mingw.org/wiki/Getting_Started/). You can compile Assimp following the instructions indicated on their [GitHub page](https://github.com/assimp/assimp/blob/master/Build.md), using the polly toolchain. Make sure compilation produces static libraries rather than shared. Make sure to compile Assimp using 32 bit MinGW. 
* Once setup, make can be called, and the executable ./EWS.exe ran with no additional parameters.
  * Note that you may have to adjust the name of the MinGW make executable. The executable may exist as mingw32-make, but can be renamed to make as necessary. However, you may or may not want to follow through with this action depending on the compilers that you have installed previously.

## License
[MIT License](https://choosealicense.com/licenses/mit/)
