# DeltaClient
A Minecraft 1.7.10 client (written in C++) with the intent of being able to be run entirely cross-platform (Windows, Mac, Linux, Emscripten).

## Dependancies
* SDL2
* SDL2_net
* [cpp-json](https://github.com/eteran/cpp-json)

## Building
Currently, DeltaClient can only be built for Windows via. MinGW. The batch file build_win_mingw.bat should be run (with modifications to successfully link to dependancies).