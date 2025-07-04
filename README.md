
Liscence ramble.----------------------------------------------------------------

I don't really care what you do with this code. For technicality purposes, it is
released under the GNU General Public Liscence v3.0. So modify it and share it, 
and learn from it to your heart's desire.

--------------------------------------------------------------------------------

Welcome to my (DK) distortion plug in. Just really doing this as a project to 
learn the JUCE framework, c++, and building with cmake. 

Steps for Building.-------------------------------------------------------------

Once you have the repository downloaded, run the command,

    cmake -Bbuild -DCMMAKE_PREFIX_PATH="PATH/TO/JUCE/INSTALL"

Note that in order to build this using CMake, you must have build JUCE using 
cmake and have the file path is to the install folder on your system. 
OR alternatively, if you want JUCE to be a subdirectory, change the 
CMakeLists.txt to add_subdirectory(JUCE).

This should create a /build subdirectory, now to build the AU, VST3, and 
standalone program, run the command,

    cmake --build build

This should also copy the AU and VST3 to the default folder where DAW's scan for
them. 
If you want to run the standalone version, follow these steps:

    cd build/DK_DISTORTION_artefacts/Standalone
    open DK_DISTORTION.app

--------------------------------------------------------------------------------

