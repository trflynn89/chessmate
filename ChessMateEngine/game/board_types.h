#pragma once

#include <cstdint>

namespace chessmate {

typedef std::uint64_t board_type;
typedef std::int16_t square_type;
typedef std::int16_t color_type;
typedef std::int16_t piece_type;
typedef std::int16_t value_type;

/**
 * Enumerated list of board files.
 */
enum Files
{
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,

    NUM_FILES
};

/**
 * Enumerated list of board ranks.
 */
enum Ranks
{
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,

    NUM_RANKS
};

/**
 * Number of squares on a board.
 */
enum BoardSize
{
    BOARD_SIZE = NUM_FILES * NUM_RANKS
};

/**
 * Enumerated list of piece colors.
 */
enum Color
{
    WHITE,
    BLACK,
    NONE
};

/**
 * Enumerated list of piece types.
 */
enum Piece
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

} // namespace chessmate

// Convert (s) to a rank
#define GET_RANK(s) ((s) / NUM_RANKS)

// Convert (s) to a file
#define GET_FILE(s) ((s) % NUM_FILES)

// Convert (r, f) to a square's index
#define GET_SQUARE(r, f) ((r)*NUM_RANKS + (f))
