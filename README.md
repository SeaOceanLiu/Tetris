# Tetris

## Features

Tetris is a cross-platform game developed using SDL3, but now it can run on Windows-x64 and Android-arm64v8. There are default total 12 kind of blocks, including ***penetration*** block and ***bomb*** block, you can extend as you like.

## ScreenShots

![cover](./doc/windows/cover.jpg)

![My best score](./doc/windows/Score.jpg)

## Download and Installation

You can download the game from [Release]([Releases · SeaOceanLiu/Tetris](https://github.com/SeaOceanLiu/Tetris/releases)).

### Windows

Run Tetris.exe directly after extracting the game to your disk. Use the keyboard arrow keys to move the block, use the space bar to rotate the block, and press the P key to pause. If it is a touch screen, you can also directly click the buttons on the screen for operation.

### Android

The Android version needs to be installed before running, and there may be a long time black screen during loading, please be patient!

## Build

We build this game on windows.

### for Windows

Visual Studio 2022 is requied to be installed before compling the game. Run ***x64 Native Tools Command Prompt for VS 2022*** from you start menu. Then you can run the following commands to build the game.

```powershell
cd ./VisualStudio/Tetris
msbuild Tetris.sln -p:Configuration=Release
```

The target files is located in ***./VisualStudio/Tetris/x64/Release***. You can change ***Release*** to ***Debug*** in the above commands if you need debug version. The debug version will launch a debug window with the game loading.

You can use following command to clean the build target file.

```powershell
msbuild Tetris.sln -t:Clean
```

### for Android

Since Android is compiled using SDL source code, it is necessary to download all the dependency libraries of SDL_ttf before executing the compilation command. Please run the following command from Windows PowerShell.

```power
cd ./Android/com.seaocean.Tetris/app/jni/SDL_ttf/external
.\Get-GitModules.ps1
```

If prompted PowerShell script permissions, simply follow the prompts to do so.

CMake is used to build this game based on ***ndkVersion 29.0.13113456***, to confirm this configuration in ***./Android/com.seaocean.Tetris/app/build.gradle***.

![build.gradle](./doc/Android/gradle.jpg)

Also you need confirm the path to the Android NDK located in ***./Android/com.seaocean.Tetris/local.properties***.

![local.properties](./doc/Android/local.jpg)

And the confirm your java.home config in ./Android/com.seaocean.Tetris/.gradle/config.properties.

![config.properties](./doc/Android/config.jpg)

Use the following command to build the game.

```powershell
cd ./AndroidStudio/Tetris/com.seaocean.Tetris
./gradlew build
```

The target files is located in ***./Android/com.seaocean.Tetris/app/build/outputs/apk***.

You can use following command to clean the build target file.

```powershell
./gradlew clean
```

## Custom block expansion configuration

The block configuration is defined in the file located as ***./core/assets/config/RBlock.jsonc***, this is a JSON format file whit comment. you can define your block in these sections:

![config define](./doc/windows/ConfigDefine.jpg)

##### groupId

Defines the group number of the block, which should be incremented in order from 0.

##### type

Defines block types, currently there are only three types of blocks: **0 for regular blocks**, **1 for penetrating blocks**, and **2 for bomb blocks**.

##### canRotate

defines whether a block can be rotated, usually it should be ***true***, but if a certain type of block should not be rotated, this value should be set to ***false***. For example, the default definition of penetrating blocks and bomb blocks cannot be rotated, as they are both blocks with only one unit.

##### define

It is a 5x5 two-dimensional matrix that defines the specific appearance of a block. A value of 0 in the matrix indicates that there is no block at that location, while a non-zero value indicates that there is a block at that location. The rotation of a block revolves around the center point of a 5x5 matrix, so the rotation center of the block should be placed at the center point of the 5x5 matrix.
