#include <vector>
#include <map>
#include "chess_notation.hpp"
#include "chess_board_constants.hpp"

namespace cbn
{
    using namespace chess_notation;
    using namespace chess_board_constants;

    enum class Piece
    {
        Pawn, Rook, Knight, Bishop, Queen, King
    };

    const std::map<const value_type, const Piece> piece_map
    {
        {BLACK_PAWN, Piece::Pawn}, {BLACK_ROOK, Piece::Rook}, {BLACK_KNIGHT, Piece::Knight}, 
        {BLACK_BISHOP, Piece::Bishop}, {BLACK_QUEEN, Piece::Queen}, {BLACK_KING, Piece::King}, 
        {WHITE_PAWN, Piece::Pawn}, {WHITE_ROOK, Piece::Rook}, {WHITE_KNIGHT, Piece::Knight}, 
        {WHITE_BISHOP, Piece::Bishop}, {WHITE_QUEEN, Piece::Queen}, {WHITE_KING, Piece::King}, 
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
                :board(b), location(l)   {   }

            const container_type<value_type, allocator_type<value_type>> operator()()
            // return a container containing all legal moves for kind located at location
            {
                container_type<value_type, allocator_type<value_type>> legal_moves{};

                value_type kind = board[location];
                Piece type = piece_map.at(kind);

                if (type == Piece::Pawn)
                {
                    // check if starting rank for WHITE and BLACK
                    // if yes return 2 front squares
                    // return 1 front square
                }

                if (type == Piece::Rook)
                {
                    // iterate up, down, left, right
                }

                if (type == Piece::Knight)
                {
                    // iterate up, down, left ,right
                }

                if (type == Piece::Bishop)
                {
                    // iterate left and right
                }

                if (type == Piece::Queen)
                {
                    // make up some smart stuff here, code later
                }

                return legal_moves;
            }
            
        private:
            const ChessBoard& board;
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
