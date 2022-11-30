# OpenGL-Parallel-Programming
A project to play around with open gl's parallel programming capabilities. Currently a basic particle physics sim.

This was originally supposed to be a fluid sim, but sadly I'm not smart enough to understand the math behind that yet, so this is just a particle physics demo for now.
In the future I might optimize this, currently the cpu is doing too many calculations (neighbour checking for example).

## Screenshots:

![Screenshot (190)](https://user-images.githubusercontent.com/86686062/204680011-addfd0f9-ff16-493b-bd46-93c371c2e3e4.png)

## Build:

To build, download the repo and double click the GenerateProjectFiles.bat file to generate the sln. This project uses premake as it's build system.
