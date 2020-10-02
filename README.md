# RayTracer
Assignment 2 for CSE-8433

## Cloning
When cloning the project it is necessary to recursively clone the submodules associated with it (glm, cimg).

The command is formatted as: git clone --recurse-submodules "the url of this repo"

This should hopefully initialize the submodules. If the cimg, glm, and tinyObj folders inside of the RayTracer folder are empty then the submodules did not clone. If this occurs then the commands

git submodule init
and
git submodule update

will remedy this. The project should now be successfully cloned.

## Solution Generation
If using Visual Studio 2019 generating the solution file is as simple as double-clicking the GenerateProject.bat file in the root directory. If using another version of visual studio premake will need
to be called with the correct argument from the command prompt in the root directory. For example:

premake\premake5.exe vs2017

This will generate a solution file for visual studio 2017.

## Compilation
Compiling the code should be as simple as selecting "rebuild solution" from the build menu. However, if this fails it is possible that the solution needs to be retargted to a different version of the Windows SDK.
This happened to me when using vs2017 instead of vs2019. To retarget the solution right-click on "Solution 'RayTracer'" in the solution explorer and select "Retarget Solution" and select the newest SDK version available to you.
Once retargeted the solution should build as mentiond previously.

## Execution
To execute the code right-click on "Test Program" in the solution explorer and start a new debug instance. The scene that is currently configured takes my system less than 20 seconds to render.