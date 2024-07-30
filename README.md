# Build for linux

## Requirements
Install SFML Library

### For Ubuntu
`sudo apt-get install libsfml-dev`

## Install

1. `git clone https://github.com/nsidir/Graphs.git`
2. `cd Graphs/Graphs`
3. `g++ -Wall -Wextra -I/usr/include/SFML Graphs.o Main.o Scenes.o -o graphs -lsfml-graphics -lsfml-window -lsfml-system`
4. `./graphs`
