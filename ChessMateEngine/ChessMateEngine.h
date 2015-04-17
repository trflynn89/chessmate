/**
 * Main header file for the application. Contains type definitions and game
 * constants.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
#pragma once

#include <cstdint>

#include <Util/Utilities.h>

// Typedefs for the board
typedef uint64_t bitboard_t;
typedef short square_t;
typedef short color_t;
typedef short piece_t;
typedef short value_t;

// Socket constants
#define CHESSMATE_PORT 12389

// Files
const square_t file_A = 0;
const square_t file_B = 1;
const square_t file_C = 2;
const square_t file_D = 3;
const square_t file_E = 4;
const square_t file_F = 5;
const square_t file_G = 6;
const square_t file_H = 7;

// Ranks
const square_t rank_1 = 0;
const square_t rank_2 = 1;
const square_t rank_3 = 2;
const square_t rank_4 = 3;
const square_t rank_5 = 4;
const square_t rank_6 = 5;
const square_t rank_7 = 6;
const square_t rank_8 = 7;

// Colors
const color_t color_W = 0; // White
const color_t color_B = 1; // Black
const color_t color_N = 2; // Neither

// Pieces
const piece_t piece_P = 0;
const piece_t piece_N = 1;
const piece_t piece_B = 2;
const piece_t piece_R = 3;
const piece_t piece_Q = 4;
const piece_t piece_K = 5;

// Difficulty
const value_t difficulty_E = 0;
const value_t difficulty_M = 1;
const value_t difficulty_H = 2;

// Board configuration
#define NUM_RANKS 8
#define NUM_FILES 8
#define BOARD_SIZE 64

// Conversions
#define GET_RANK(s) ((s) / NUM_RANKS) // Convert (s) to a rank
#define GET_FILE(s) ((s) % NUM_FILES) // Convert (s) to a file
#define GET_SQUARE(r, f) ((r) * NUM_RANKS + (f)) // Convert (r, f) to a square's index

// Get the least significant bit
inline square_t firstBit(bitboard_t b)
{
	square_t s = 0;
	while ((b >> s) % 2 == 0)
	{
		++s;
	}
	return s;
}

// Get the most significant bit
inline square_t lastBit(bitboard_t b)
{
	square_t s = 0;
	while (b >> s)
	{
		++s;
	}
	return s;
}

// Max sizes for static arrays
#define MAX_NUM_MOVES		8
#define MAX_NUM_MOVES_TOT	512 // TODO determine best size
