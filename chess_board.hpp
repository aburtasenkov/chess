#include <vector>
#include <map>
#include <array>
#include <algorithm>
#include "chess_notation.hpp"
#include "chess_board_constants.hpp"

namespace cbn
{
    using namespace chess_notation;
    using namespace chess_board_constants;

    enum class Piece_type
    {
        Pawn, Rook, Knight, Bishop, Queen, King
    };

    const std::map<const value_type, const Piece_type> piece_type_map
    {
        {BLACK_PAWN, Piece_type::Pawn}, {BLACK_ROOK, Piece_type::Rook}, {BLACK_KNIGHT, Piece_type::Knight}, 
        {BLACK_BISHOP, Piece_type::Bishop}, {BLACK_QUEEN, Piece_type::Queen}, {BLACK_KING, Piece_type::King}, 
        {WHITE_PAWN, Piece_type::Pawn}, {WHITE_ROOK, Piece_type::Rook}, {WHITE_KNIGHT, Piece_type::Knight}, 
        {WHITE_BISHOP, Piece_type::Bishop}, {WHITE_QUEEN, Piece_type::Queen}, {WHITE_KING, Piece_type::King}, 
    };

    enum class Piece_color
    {
        White, Black
    };

    const std::array<const value_type, 6> piece_color_white_vector
    {
        WHITE_PAWN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING
    };

    struct Piece_data
    {
        value_type value;   // the character
        Piece_type type;
        Piece_color color;
    };


    class ChessBoard{

        public:
            friend std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

            void restore();

            void move(const ChessNotation& xy)
            {
                value_type& square_ref = board[xy.x.integer][xy.x.character];
                board[xy.y.integer][xy.y.character] = square_ref;
                square_ref = EMPTY_SQUARE;
            }

            value_type& operator[](const ChessCoordinate& location)
            {
                return board[location.integer][location.character];
            }

            const value_type& operator[](const ChessCoordinate& location) const
            {
                return board[location.integer][location.character];
            }

        private:
            bn::Board<container_type, value_type, allocator_type> board{DEFAULT_CHESS_BOARD};

    };

    class Legalmoves
    {
        public:
            Legalmoves(const ChessBoard& b, const ChessCoordinate& l)
                :board(b), location(l)   
                {
                    piece_info.value = board[location];

                    if (piece_info.value == EMPTY_SQUARE)
                        throw EmptySquareError;

                    piece_info.type = piece_type_map.at(piece_info.value);

                    // if in piece_color_white_vector --> white, else black
                    if (std::find(piece_color_white_vector.begin(), piece_color_white_vector.end(), piece_info.value) != piece_color_white_vector.end())
                        piece_info.color = Piece_color::White;
                    else 
                        piece_info.color = Piece_color::Black;
                }

            const container_type<value_type, allocator_type<value_type>> operator()()
            // return a container containing all legal moves for kind located at location
            {
                container_type<value_type, allocator_type<value_type>> legal_moves{};

                if (piece_info.type == Piece_type::Pawn)
                {
                    // check if starting rank for WHITE and BLACK
                    // if yes return 2 front squares
                    // return 1 front square
                }

                if (piece_info.type == Piece_type::Rook)
                {
                    // iterate up, down, left, right
                }

                if (piece_info.type == Piece_type::Knight)
                {
                    // iterate left, right
                }

                if (piece_info.type == Piece_type::Bishop)
                {
                    // iterate left and right
                }

                if (piece_info.type == Piece_type::Queen)
                {
                    // make up some smart stuff here, code later
                }

                return legal_moves;
            }
            
        private:
            const ChessBoard& board;
            Piece_data piece_info;
            ChessCoordinate location;
    };

    // Function and Method Definition

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb)
    {
        return os << cb.board;
    }

    void ChessBoard::restore()
    {
        board = DEFAULT_CHESS_BOARD;
    }
}
