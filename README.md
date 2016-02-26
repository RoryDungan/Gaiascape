#Gaiascape
Gaiascape is a program that gives artists and designers the ability to quickly and easily generate terrains 
and landscapes.

Originally designed to showcase the terrain generation algorithm designed for a separate game project, 
Gaiascape provides an easy to use interface for proceduraly generating random landscapes according to set 
parameters, as well as the ability to edit existing terrain manually.

## Building
Gaiascape is designed to build on Windows with Qt 4.8.6 and MinGW GCC 4.8.2.

To build, make sure you have the correct version of MiinGW, available here:
[mingw-builds i686-4.8.2-release-posix-dwarf-rt_v3-rev3](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z/download)

Then install download and install Qt 4.8.6 for Windows MinGW (`qt-opensource-windows-x86-mingw482-4.8.6-1.exe`) 
from the Qt archive: http://download.qt.io/archive/qt/4.8/4.8.6/qt-opensource-windows-x86-mingw482-4.8.6-1.exe

Set up a Qt configuration for Qt 4.8.6 with GCC 4.8.2 in Qt Creator, then load up `TerrainEdit.pro`, and you
should be ready to build Gaiascape.


## Rebuilding Ogre

The repository includes a prebuilt copy of Ogre 1.8.1 for Windows, although in the case that you want to 
rebuild it from source, this is how to do it.

### Tools required
 - CMake gui: https://cmake.org/download/
 - DirectX SDK (needed to compile Ogre): https://www.microsoft.com/en-us/download/details.aspx?id=6812

### Boost
Follow the instructions on http://www.boost.org/ to build Boost 1.60.0

### ogredeps
Go to https://bitbucket.org/cabalistic/ogredeps and download or clone the repository. Copy to 
`C:\dev\ogredeps`.

We will compile first in release mode and then in debug mode. 

*   Open CMake GUI and change
    *   Where the source code is: `C:/dev/ogredeps`
    *   Where the library will be built: `C:/dev/ogredeps/build/rel`
*   Click "Configure" and choose "MinGW Makefiles". You may need to then tell CMake where to find `gcc`, `g++`
   and `mingw32-make`.
*   Set `CMAKE_BUILD_TYPE` to `Release`
*   Set `CMAKE_INSTALL_PREFIX` to `C:/dev/ogredeps/build/inst`
*   Click "Configure" again and then "Generate"
*   In a command prompt:
    *   `PATH=%PATH%;C:\mingw32\bin`
    *   `cd C:\dev\ogredeps\build\rel`
    *   `mingw32-make install`
   
Then we compile in debug mode
*   Go back to CMake GUI and change where teh library will be built to `C:/dev/ogredeps/build/dbg`
*   Click "Configure"
*   Set `CMAKE_BUILD_TYPE` to `Debug`
*   Set `CMAKE_INSTALL_PREFIX` to `C:/dev/ogredeps/build/inst`
*   Click "Configure" again and then "Generate"
*   In a command prompt:
    *   `PATH=%PATH%;C:\mingw32\bin`
    *   `cd C:\dev\ogredeps\build\dbg`
    *   `mingw32-make install`

### Ogre

Download Ogre 1.8.1 source from the Ogre archives: 
https://sourceforge.net/projects/ogre/files/ogre/1.8/1.8.1/ogre_src_v1-8-1.exe/download
Extract to `C:\dev\ogre-1.8.1-src`

For the release build: 
*   Open CMake GUI and change
    *   Where the source code is: `C:/dev/ogre-1.8.1-src`
    *   Where the library will be built: `C:/dev/ogre-1.8.1/build/rel`
*   Click "Configure"
*   Set `CMAKE_BUILD_TYPE` to `Release`
*   Set `CMAKE_INSTALL_PREFIX` to `C:/dev/ogre-1.8.1/sdk`
*   Set `OGRE_DEPENDENCIES_DIR` to `C:/dev/ogredeps/build/inst`
*   Set `Boost_INCLUDE_DIR` to `C:/dev/boost_1_60_0`
*   Uncheck `OGRE_BUILD_RENDERSYSTEM_D3D9`
*   Click "Configure" and then "Generate"
*   In a command prompt:
    *   `cd C:\dev\ogre-1.8.1\build\rel`
    *   `mingw32-make install`

Then for the debug build:
*   Open CMake GUI and change
    *   Where the source code is: `C:/dev/ogre-1.8.1-src`
    *   Where the library will be built: `C:/dev/ogre-1.8.1/build/dbg`
*   Click "Configure"
*   Set `CMAKE_BUILD_TYPE` to `Debug`
*   Set `CMAKE_INSTALL_PREFIX` to `C:/dev/ogre-1.8.1/sdk`
*   Set `OGRE_DEPENDENCIES_DIR` to `C:/dev/ogredeps/build/inst`
*   Set `Boost_INCLUDE_DIR` to `C:/dev/boost_1_60_0`
*   Uncheck `OGRE_BUILD_RENDERSYSTEM_D3D9`
*   Click "Configure" and then "Generate"
*   In a command prompt:
    *   `cd C:\dev\ogre-1.8.1\build\dbg`
    *   `mingw32-make install`
   
