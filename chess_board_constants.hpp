#include "Board.hpp"

template <typename T, typename A>
using container_type = std::vector<T, A>;
    
template <typename T>
using allocator_type = std::allocator<T>;
    
using value_type = std::string;     // std::string because it is not bound to 1 byte of data --> no overflow

const int CHESS_BOARD_SIZE = 8;
const value_type EMPTY_SQUARE = "□";

const value_type WHITE_KING = "♔";
const value_type WHITE_QUEEN = "♕";
const value_type WHITE_ROOK = "♖";
const value_type WHITE_BISHOP = "♗";
const value_type WHITE_KNIGHT = "♘";
const value_type WHITE_PAWN = "♙";

const value_type BLACK_KING = "♚";
const value_type BLACK_QUEEN = "♛";
const value_type BLACK_ROOK = "♜";
const value_type BLACK_BISHOP = "♝";
const value_type BLACK_KNIGHT = "♞";
const value_type BLACK_PAWN = "♟";

const bn::Board<container_type, value_type, allocator_type>::row_type RANK_1{ "1", BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, 
                                                                                BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK };

const bn::Board<container_type, value_type, allocator_type>::row_type RANK_2{ "2", BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, 
                                                                                BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, };

const bn::Board<container_type, value_type, allocator_type>::row_type RANK_3{ "3", EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 
                                                                            EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE };

const bn::Board<container_type, value_type, allocator_type>::row_type RANK_4{ "4", EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 
                                                                            EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE };

const bn::Board<container_type, value_type, allocator_type>::row_type RANK_5{ "5", EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 
                                                                            EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE };

const bn::Board<container_type, value_type, allocator_type>::row_type RANK_6{ "6", EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 
                                                                            EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE };

const bn::Board<container_type, value_type, allocator_type>::row_type RANK_7{ "7", WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, 
                                                                                WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN };
                                                                            
const bn::Board<container_type, value_type, allocator_type>::row_type RANK_8{ "8", WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, 
                                                                                WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK };

const bn::Board<container_type, value_type, allocator_type>::row_type CHAR_COORDINATE_RANK{ "O", "A", "B", "C", "D", 
                                                                                "E", "F", "G", "H" };

const bn::Board<container_type, value_type, allocator_type>::board_type DEFAULT_CHESS_BOARD
{
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
    CHAR_COORDINATE_RANK
};