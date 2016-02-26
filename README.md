# Gaiascape
Gaiascape is a program that gives artists and designers the ability to quickly and easily generate terrains 
and landscapes.

Originally designed to showcase the terrain generation algorithm designed for a separate game project, 
Gaiascape provides an easy to use interface for proceduraly generating random landscapes according to set 
parameters, as well as the ability to edit existing terrain manually.

## Building
Gaiascape is designed to build on Windows with Qt 4.8.6 and MinGW GCC 4.8.2.

### Tools required
 - MinGW GCC 4.8.2: [mingw-builds i686-4.8.2-release-posix-dwarf-rt_v3-rev3](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z/download)
 - Qt 4.8.6 for MinGW GCC 4.8.2: http://download.qt.io/archive/qt/4.8/4.8.6/qt-opensource-windows-x86-mingw482-4.8.6-1.exe
 - CMake gui (used for compiling Ogre and its dependencies): https://cmake.org/download/
 - DirectX SDK (needed to compile Ogre): https://www.microsoft.com/en-us/download/details.aspx?id=6812
 - 7-zip (for extracting downloads): http://www.7-zip.org/download.html

Extract MinGW (`i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z`) to C:\ and then go into Qt Creator to register 
it. Go to Tools -> Options -> Build & Run -> Compilers, click "Add", then "MinGW". This will add another 
compiler under the "Manual" section. Select it, and in the "Compiler path:" field enter `C:\mingw32\bin\g++.exe`
and in the "Name" field enter "MinGW32 4.8.2".

Next, go to the "Debuggers" tab, click "Add", and set the "Path" field of the new compiler to 
`C:\mingw32\bin\gdb.exe` and the "Name" field to "MinGW32 4.8.2 GDB".

Ensure that "Qt 4.8.6" is listed under the "Qt Versions" tab. If not, click "Add..." and navigate to the qmake.exe
executable, which should be located at `C:\Qt\4.8.6\bin\qmake.exe`. 

Finally, go to the "Kits" tab and click "Add". Set the compiler to "MinGW32 4.8.2", set the debugger to "MinGW32
4.8.2 GDB", and set the Qt version to 4.8.6.

Now that we've set up our environment we'll need to build Gaiascape's dependencies.

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
   
### Building Gaiascape

Finally, now that our build environment is configured and dependencies built, we can build Gaiascape. 

Open `TerrainEdit.pro` in Qt Creator. You will be prompted to configure the project - select the Qt kit
we created earlier for Qt 4.8.6 with MinGW32 4.8.2 and click "Configure project". This will set up the user 
configuration file for the project but we still need to tell it where to look for the dependencies. Switch to
the "Projects" view on the left, select "Build and Run" and under that "Build". Expand the `Build Environment`
section to see all the environment variables used when building and running the project and add the following:
 - `OGRE_HOME`: `C:\dev\ogre-1.8.1\sdk`
 - `BOOST_HOME`: `C:\dev\boost_1_60_0`
Repeat this process for both the Debug and Release builds (selectable by clicking on the computer icon in the
bottom left corner of the window). Alternatively, you can set these up as system environment variables rather
than including it just in the environment that gets set up when QMake is invoked. 
 
Hit the green triangle on the left or press Ctrl-R to build and run the application!