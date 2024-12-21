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

    // Function and Method Definition

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb)
    {
        return os << cb.board;
    }

    void ChessBoard::restore()
    {
        board = DEFAULT_CHESS_BOARD;
    }

    bool square_is_empty(const value_type& val)
    {
        return val == EMPTY_SQUARE;
    }
}

namespace lmn
{
    using namespace cbn;

    class Legalmoves
    {
        public:
            Legalmoves(const ChessBoard& b)
                :board(b)
            {   }
            // calculate all legal moves for piece at location
            const container_type<ChessCoordinate, allocator_type<ChessCoordinate>> operator()(const ChessCoordinate& location);
            
        private:
            const ChessBoard& board;
            Piece_data get_piece_info(const ChessCoordinate& location);
            void append_legalmoves_pawn(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);
    };

    Piece_data Legalmoves::get_piece_info(const ChessCoordinate& location)
    {
        Piece_data piece_info;

        // make piece_info data
        {
        piece_info.value = board[location];

        if (square_is_empty(board[location]))
            throw EmptySquareError;

        piece_info.type = piece_type_map.at(piece_info.value);

        // if in piece_color_white_vector --> white, else black
        if (std::find(piece_color_white_vector.begin(), piece_color_white_vector.end(), piece_info.value) != piece_color_white_vector.end())
            piece_info.color = Piece_color::White;
        else 
            piece_info.color = Piece_color::Black;
        }

        return piece_info;
    }

    void Legalmoves::append_legalmoves_pawn(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location)
    // append legal_moves with possible moves for pawns
    {
        // Check for en passant, TODO

        ChessCoordinate front = location - ChessCoordinate{0,1};    // coordinate in front of pawn
        ChessCoordinate front_2 = front - ChessCoordinate{0,1};     // coordinate 2 squares in front of pawn
        Piece_color color = Piece_color::White;
        int starting_row = RANK_7_INDEX;

        // Do once for white, once for black
        for (int _ = 0; _ < 2; ++_)
        {
            if (piece_info.color == color)
            {
                if (square_is_empty(board[front]))
                {
                    legal_moves.push_back(front);
                    
                    if (location.integer == starting_row && square_is_empty(board[front_2]))    // pawns on starting row can move 2 squares
                        legal_moves.push_back(front_2);
                }
                return;
            }

            color = Piece_color::Black;
            starting_row = RANK_2_INDEX;
            front = location + ChessCoordinate{0,1};
            front_2 = front + ChessCoordinate{0,1};
        }
        return;
    }
        
    const container_type<ChessCoordinate, allocator_type<ChessCoordinate>> Legalmoves::operator()(const ChessCoordinate& location)
    // return a container containing all legal moves for kind located at location
    {
        Piece_data piece_info = get_piece_info(location);

        container_type<ChessCoordinate, allocator_type<ChessCoordinate>> legal_moves{};

        if (piece_info.type == Piece_type::Pawn)
            append_legalmoves_pawn(legal_moves, piece_info, location);

        else if (piece_info.type == Piece_type::Rook)
        {
            // iterate up, down, left, right
        }

        else if (piece_info.type == Piece_type::Knight)
        {
            // iterate left, right
        }

        else if (piece_info.type == Piece_type::Bishop)
        {
            // iterate left and right
        }

        else if (piece_info.type == Piece_type::Queen)
        {
            // make up some smart stuff here, code later
        }

        return legal_moves;
    }
};