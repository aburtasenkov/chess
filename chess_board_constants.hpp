#include <map>
#include <array>
#include "Board.hpp"

namespace chess_types
{
    template <typename T, typename A>
    using container_type = std::vector<T, A>;
        
    template <typename T>
    using allocator_type = std::allocator<T>;
        
    using value_type = std::string;     // std::string because it is not bound to 1 byte of data --> no overflow
}

namespace helper_classes
{
    enum class Piece_type
    {
        Pawn, Rook, Knight, Bishop, Queen, King, Empty
    };

    enum class Piece_color
    {
        White, Black, Neutral
    };

    struct Piece{
        chess_types::value_type character; // io character
        Piece_type type;
        Piece_color color;

        Piece()
        :character("□"), type(Piece_type::Empty), color(Piece_color::Neutral)
        {   }

        Piece(const chess_types::value_type& val, const Piece_type& t, const Piece_color& c)
        :character(val), type(t), color(c)
        {   }
    };

    std::ostream& operator<<(std::ostream& os, const Piece& piece)
    {
        return os << piece.character;
    }
}

namespace chess_constants
{
    
    const Exception IllegalMoveError{"IllegalMoveError: The Inputted Move Is Illegal To Do"};
    const Exception BadSequenceError{"BadSequenceError: This Color is Not At Move"};
    const Exception KingIsCheckedError{"KingIsCheckedError: The Inputted Move Is Illegal To Do"};

    const std::map<helper_classes::Piece_color, helper_classes::Piece_color> enemy_color
    {
        {helper_classes::Piece_color::White, helper_classes::Piece_color::Black}, {helper_classes::Piece_color::Black, helper_classes::Piece_color::White}
    };

    const int CHESS_BOARD_SIZE = 8;
    const helper_classes::Piece EMPTY_SQUARE{"□", helper_classes::Piece_type::Empty, helper_classes::Piece_color::Neutral};

    const helper_classes::Piece WHITE_KING{"♔", helper_classes::Piece_type::King, helper_classes::Piece_color::White};
    const helper_classes::Piece WHITE_QUEEN{"♕", helper_classes::Piece_type::Queen, helper_classes::Piece_color::White};
    const helper_classes::Piece WHITE_ROOK{"♖", helper_classes::Piece_type::Rook, helper_classes::Piece_color::White};
    const helper_classes::Piece WHITE_BISHOP{"♗", helper_classes::Piece_type::Bishop, helper_classes::Piece_color::White};
    const helper_classes::Piece WHITE_KNIGHT{"♘", helper_classes::Piece_type::Knight, helper_classes::Piece_color::White};
    const helper_classes::Piece WHITE_PAWN{"♙", helper_classes::Piece_type::Pawn, helper_classes::Piece_color::White};

    const helper_classes::Piece BLACK_KING{"♚", helper_classes::Piece_type::King, helper_classes::Piece_color::Black};
    const helper_classes::Piece BLACK_QUEEN{"♛", helper_classes::Piece_type::Queen, helper_classes::Piece_color::Black};
    const helper_classes::Piece BLACK_ROOK{"♜", helper_classes::Piece_type::Rook, helper_classes::Piece_color::Black};
    const helper_classes::Piece BLACK_BISHOP{"♝", helper_classes::Piece_type::Bishop, helper_classes::Piece_color::Black};
    const helper_classes::Piece BLACK_KNIGHT{"♞", helper_classes::Piece_type::Knight, helper_classes::Piece_color::Black};
    const helper_classes::Piece BLACK_PAWN{"♟", helper_classes::Piece_type::Pawn, helper_classes::Piece_color::Black};

    const bn::Board<chess_types::container_type, helper_classes::Piece, chess_types::allocator_type>::row_type BLACK_PIECES_RANK{ BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, 
                                                                                    BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK };

    const bn::Board<chess_types::container_type, helper_classes::Piece, chess_types::allocator_type>::row_type BLACK_PAWN_RANK{ BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, 
                                                                                    BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, };

    const bn::Board<chess_types::container_type, helper_classes::Piece, chess_types::allocator_type>::row_type EMPTY_RANK{ EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 
                                                                                EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE };

    const bn::Board<chess_types::container_type, helper_classes::Piece, chess_types::allocator_type>::row_type WHITE_PAWN_RANK{ WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, 
                                                                                    WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN };
                                                                                
    const bn::Board<chess_types::container_type, helper_classes::Piece, chess_types::allocator_type>::row_type WHITE_PIECES_RANK{ WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, 
                                                                                    WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK };

    const bn::Board<chess_types::container_type, chess_types::value_type, chess_types::allocator_type>::row_type CHAR_COORDINATE_RANK{ "O", "A", "B", "C", "D", 
                                                                                    "E", "F", "G", "H" };

    const bn::Board<chess_types::container_type, helper_classes::Piece, chess_types::allocator_type>::board_type DEFAULT_CHESS_BOARD
    {
        BLACK_PIECES_RANK, BLACK_PAWN_RANK, EMPTY_RANK, EMPTY_RANK, EMPTY_RANK, EMPTY_RANK, WHITE_PAWN_RANK, WHITE_PIECES_RANK
    };

    const int KNIGHT_OFFSET_LONG_SIDE = 2;
    const int KNIGHT_OFFSET_SHORT_SIDE = 1;
    const int BISHOP_OFFSET = 1;
    const int KING_OFFSET_DIAGONAL = 1;
    const int KING_OFFSET_CROSSWAYS = 0;
    const int ROOK_OFFSET_X = 1;
    const int ROOK_OFFSET_Y = 0;
    const int PAWN_OFFSET_X = 0;
    const int PAWN_OFFSET_Y = 1;
    const int CASTLE_OFFSET = 2;

    const int LEFT_ROOK_CHARACTER = 0;
    const int RIGHT_ROOK_CHARACTER = 7;

    const int LEFT_CASTLE_CHARACTER = 2;
    const int RIGHT_CASTLE_CHARACTER = 6;

    const std::map<helper_classes::Piece_type, int> piece_score
    {
        {helper_classes::Piece_type::Pawn, 1},
        {helper_classes::Piece_type::Knight, 3},
        {helper_classes::Piece_type::Bishop, 3},
        {helper_classes::Piece_type::Rook, 5},
        {helper_classes::Piece_type::Queen, 9},
    };
}

namespace chess_pieces
{
    using namespace chess_types;
    using namespace helper_classes;
    using namespace chess_constants;
}