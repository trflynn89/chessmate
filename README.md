# ChessMate

ChessMate is a chess engine written in C++ with a game-playing GUI written in Java. Pretty incomplete at this point as I only get to work on it sporadically.

## ChessMateEngine

The engine uses a bitboard representation of the chess board and uses an iterative min-max algorithm with alpha/beta pruning to decide what move to make. Behind the scenes, an asynchronous socket system and a messaging system is used to communicate to clients.

Builds and runs on Windows (Visual Studio 2015), CentOS (gcc 5.1.1), Ubuntu (gcc 5.3.1), and Antergos (gcc 5.3.0).

## ChessMateGUI

![alt tag](http://i.imgur.com/xOpjLJJ.png)
