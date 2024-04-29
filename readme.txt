Install the latest version of MinGW-w64 via MSYS2
 
actual version MinGW-w64 version use: gcc-g++ versions (Rev3, Built by MSYS2 project) 13.2.0

actual sdl version use: https://github.com/libsdl-org/SDL/releases/download/release-2.30.2/SDL2-devel-2.30.2-mingw.zip

To compile use this command: g++ -Llib -o game  main.cpp -lSDL2
To run: ./game

To build and run automatically:
powershell: ./run 
bash: ./run.bash  