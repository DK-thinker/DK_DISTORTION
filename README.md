
Liscence.----------------------------------------------------------------

I don't really care what you do with this code. For technicality purposes, it is
released under the GNU General Public Liscence v3.0. So modify it and share it, 
and learn from it to your heart's desire. The liscense detail is in this repo 

--------------------------------------------------------------------------------

Welcome to my (DK) distortion plug in. Just really doing this as a project to 
learn the JUCE framework, c++, and building with cmake. 

Steps for Building.-------------------------------------------------------------

Requierments:
CMake
JUCE

Once you have the repository downloaded, run the command,

    cmake -Bbuild -DCMMAKE_PREFIX_PATH=PATH/TO/JUCE/INSTALL

*Note that in order to build this using CMake, you must have built JUCE using 
cmake and have the file path is to the install folder on your system. More can
be found about CMake and JUCE here: https://ccrma.stanford.edu/~jos/juce_modules/md__Users_jos_w_JUCEModulesDoc_docs_CMake_API.html

This should create a /build subdirectory, now to build the AU, VST3, and 
standalone program, run the command,

    cmake --build build

This should also copy the AU and VST3 to the default folder where DAW's scan for
them. 
If you want to run the standalone version, follow these steps:

    cd build/DK_DISTORTION_artefacts/Standalone
    open DK_DISTORTION.app

If you wish to modify the cmake process, edit the CMakeLists.txt as desired. 
Plenty of information can be found on the link above, and any cmake documentation

--------------------------------------------------------------------------------

