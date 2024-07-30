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

# Graph Application User Guide

This application allows you to create and manipulate graphs interactively. Here's how to use the main features:

## Node Creation
- **Right Click**: Create a new node at the cursor position

## Edge Creation
- **Left Click and Drag**: Connect two nodes by clicking on one node, dragging to another, and releasing

## Information Display
- Press **i key**: View information about the current graph

## Clear Graph
- Press **c key**: Remove all nodes and edges, resetting the graph

## Undo Action
- Press **Ctrl + z**: Revert the last action performed
