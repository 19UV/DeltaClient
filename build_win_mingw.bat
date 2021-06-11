@echo off

g++ src/main.cpp src/recv_buffer.cpp^
 -W -Wall^
 -ID:/cpp-json-4.1/lib/include^
 -ID:/SDL2-2.0.14/include -LD:/SDL2-2.0.14/lib^
 -lmingw32 -lSDL2main -lSDL2 -lSDL2_net^
 -o DeltaClient.exe