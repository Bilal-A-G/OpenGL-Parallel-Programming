# OpenGL-Parallel-Programming
A project to play around with open gl's parallel programming capabilities. Currently a basic particle physics sim.

This was originally supposed to be a fluid sim, but sadly I'm not smart enough to make that yet (in OpenGL), so this is just a particle physics demo for now.
In the future I might optimize this, currently the cpu is doing too many calculations (neighbour checking for example).

## Controls:

WASD - Move

Right click and drag - Look

## Features:

- **Neighbour checking via spatial hashing**
- **Impulse based collision resolution**
- **GPU instancing for drawing particles**
- **GPU Physics**
- **65K+ particles simulated**

## Media:

![](https://github.com/Bilal-A-G/OpenGL-Parallel-Programming/blob/main/Media/Particle%20Demo.gif)

## Build:

To build, download the repo and double click the GenerateProjectFiles.bat file to generate the sln. This project uses premake as it's build system.
