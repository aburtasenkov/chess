#include <vector>
#include "Board.hpp"

namespace chess_board_namespace
{
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

    const Board<container_type, value_type, allocator_type>::row_type BLACK_BACK_RANK{ "1", BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, 
                                                                                BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK };

    const Board<container_type, value_type, allocator_type>::row_type BLACK_PAWN_RANK{ "2", BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, 
                                                                                BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, };

    const Board<container_type, value_type, allocator_type>::row_type EMPTY_RANK{ "3", EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 
                                                                            EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE };

    const Board<container_type, value_type, allocator_type>::row_type WHITE_PAWN_RANK{ "7", WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, 
                                                                                WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN };
                                                                            
    const Board<container_type, value_type, allocator_type>::row_type WHITE_BACK_RANK{ "8", WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, 
                                                                                WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK };

    const Board<container_type, value_type, allocator_type>::row_type CHAR_COORDINATE_RANK{ "O", "A", "B", "C", "D", 
                                                                                "E", "F", "G", "H" };

    const Board<container_type, value_type, allocator_type>::board_type DEFAULT_CHESS_BOARD
    {
        BLACK_BACK_RANK, BLACK_PAWN_RANK,
        EMPTY_RANK, EMPTY_RANK, EMPTY_RANK,EMPTY_RANK,
        WHITE_PAWN_RANK, WHITE_BACK_RANK,
        CHAR_COORDINATE_RANK
    };

    class ChessBoard{

        public:
            friend std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

            void restore();

        private:
            Board<container_type, value_type, allocator_type> board{DEFAULT_CHESS_BOARD};

    };

    // Function and Method Definition

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb)
    {
        return os << cb.board;
    }
}