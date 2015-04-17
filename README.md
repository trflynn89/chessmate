# ChessMate

ChessMate is a chess engine written in C++ with a game-playing GUI written in Java. Pretty incomplete at this point as I only get to work on it sporadically.

## ChessMateEngine

The engine uses a bitboard representation of the chess board and uses an iterative min-max algorithm with alpha/beta pruning to decide what move to make. Behind the scenes, an asynchronous socket system and a messaging system is used to communicate to clients.

It was originally written to build on both windows (visual studio) and linux (g++). The current state is that it builds on windows with MinGW-64, and needs to be fixed to work on linux again.

To build, simply cd to the ChessMateEngine/Build directory and run "make" or "make release=1".

## ChessMateGUI

![alt tag](http://i.imgur.com/xOpjLJJ.png)
