#include <vector>
#include <array>
#include <algorithm>
#include "chess_notation.hpp"
#include "chess_board_constants.hpp"

namespace cbn
{
    using namespace chess_notation;
    using namespace chess_board_constants;
    using coordinate_container = container_type<ChessCoordinate, allocator_type<ChessCoordinate>>;

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

            void move(const ChessNotation& xy);

            value_type& operator[](const ChessCoordinate& location);

            const value_type& operator[](const ChessCoordinate& location) const;

        private:
            bn::Board<container_type, value_type, allocator_type> board{DEFAULT_CHESS_BOARD};

    };

    /****************************************************Function declaration************************************************************************************/

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

    // return true if square value is empty
    bool is_empty(const value_type& val);

    // return true if movement.from is in legal_moves --> the move is legal
    bool move_is_legal(const coordinate_container& legal_moves, const ChessNotation& movement);
}

/**************************************************************************************Function definition*******************************************************************/

std::ostream& cbn::operator<<(std::ostream& os, const cbn::ChessBoard& cb)
{
    return os << cb.board;
}

void cbn::ChessBoard::restore()
{
    board = DEFAULT_CHESS_BOARD;
}

cbn::value_type& cbn::ChessBoard::operator[](const cbn::ChessCoordinate& location)
{
    return board[location.integer][location.character];
}

const cbn::value_type& cbn::ChessBoard::operator[](const cbn::ChessCoordinate& location) const
{
    return board[location.integer][location.character];
}

bool cbn::is_empty(const value_type& val)
{
    return val == EMPTY_SQUARE;
}

bool cbn::move_is_legal(const cbn::coordinate_container& legal_moves, const cbn::ChessNotation& movement)
{
    return (std::find(legal_moves.begin(), legal_moves.end(), movement.to) != legal_moves.end());
}

/****************************************************************************************************************************************/

namespace lmn
{
    class Legalmoves
    {
        public:
            Legalmoves(const cbn::ChessBoard& b)
                :board(b)   {   }
            
            // calculate all legal moves for piece at location
            const cbn::coordinate_container operator()(const cbn::ChessCoordinate& location);
            
        private:
            const cbn::ChessBoard& board;

            cbn::Piece_data get_piece_info(const cbn::ChessCoordinate& location);
            void append_legalmoves_pawn(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_legalmoves_rook(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_knight_move(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_bishop_diagonal(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_legalmoves_king(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);

            bool is_enemy(const cbn::ChessCoordinate& l1, const cbn::ChessCoordinate& l2);
    };

    cbn::container_type<std::pair<int,int>, cbn::allocator_type<std::pair<int,int>>> generate_mixes(int i1, int i2);
};

/*******************************************************************Function definition*********************************************************************/

cbn::Piece_data lmn::Legalmoves::get_piece_info(const cbn::ChessCoordinate& location)
{
    cbn::Piece_data piece_info;

    // make piece_info data
    {
    piece_info.value = board[location];

    if (cbn::is_empty(board[location]))
        throw cbn::EmptySquareError;

    piece_info.type = cbn::piece_type_map.at(piece_info.value);

    // if in piece_color_white_vector --> white, else black
    if (std::find(cbn::piece_color_white_vector.begin(), cbn::piece_color_white_vector.end(), piece_info.value) != cbn::piece_color_white_vector.end())
        piece_info.color = cbn::Piece_color::White;
    else 
        piece_info.color = cbn::Piece_color::Black;
    }

    return piece_info;
}

void lmn::Legalmoves::append_legalmoves_pawn(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
// append legal_moves with possible moves for pawns
{
    // Check for en passant, TODO

    cbn::ChessCoordinate front, front_2, potential_square_1, potential_square_2;
    int starting_row;

    if (piece_info.color == cbn::Piece_color::White)
    {
        starting_row = cbn::RANK_7_INDEX;
        front = location - cbn::ChessCoordinate{offset_x, offset_y};   // coordinate in front of pawn
        front_2 = front - cbn::ChessCoordinate{offset_x, offset_y};    // coordinate 2 squares in front of pawn
    }
    else
    {
        starting_row = cbn::RANK_2_INDEX;
        front = location + cbn::ChessCoordinate{offset_x, offset_y};   // coordinate in front of pawn
        front_2 = front + cbn::ChessCoordinate{offset_x, offset_y};    // coordinate 2 squares in front of pawn
    }

    potential_square_1 = front + cbn::ChessCoordinate{-offset_y, 0};
    potential_square_2 = front + cbn::ChessCoordinate{offset_y, 0};

    const std::array<cbn::ChessCoordinate, 2> arr = {potential_square_1, potential_square_2};

    if (cbn::is_empty(board[front]))
    {
        legal_moves.push_back(front);
        
        if (location.integer == starting_row && cbn::is_empty(board[front_2]))    // pawns on starting row can move 2 squares
            legal_moves.push_back(front_2);
    }

    for (const auto& square : arr)
    {
        if (!cbn::is_empty(board[square]) && is_enemy(location, square))
            legal_moves.push_back(square);
    }

    return;
}

void lmn::Legalmoves::append_legalmoves_rook(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate current = location + cbn::ChessCoordinate{offset_x, offset_y};

    while (current.is_valid())
    {
        if (cbn::is_empty(board[current]))
            legal_moves.push_back(current);
        else
        {
            if (is_enemy(location, current))
                legal_moves.push_back(current);
            break;
        }

        current += cbn::ChessCoordinate{offset_x, offset_y};
    }

    return;
}

void lmn::Legalmoves::append_knight_move(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate current = location + cbn::ChessCoordinate{offset_x, offset_y};

    if (!current.is_valid())
        return;

    if (cbn::is_empty(board[current]))
        legal_moves.push_back(current);
    else
    {
        if (is_enemy(location, current))
            legal_moves.push_back(current);
    }

    return;
}

void lmn::Legalmoves::append_bishop_diagonal(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate diagonal = location + cbn::ChessCoordinate{offset_x, offset_y};

    while (diagonal.is_valid())
    {
        if (cbn::is_empty(board[diagonal]))
            legal_moves.push_back(diagonal);
        else
        {
            if (is_enemy(location, diagonal))
                legal_moves.push_back(diagonal);
            break;
        }

        diagonal += cbn::ChessCoordinate{offset_x, offset_y};
    }

    return;
}

void lmn::Legalmoves::append_legalmoves_king(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate current = location + cbn::ChessCoordinate{offset_x, offset_y};
    if (current.is_valid())
    {
        if (cbn::is_empty(board[current]))
        {
            legal_moves.push_back(current);
        }
        else
        {
            if (is_enemy(location, current))
                legal_moves.push_back(current);
        }
    }
    return;
}
     
const cbn::coordinate_container lmn::Legalmoves::operator()(const cbn::ChessCoordinate& location)
// return a container containing all legal moves for kind located at location
{
    cbn::Piece_data piece_info = get_piece_info(location);

    cbn::coordinate_container legal_moves{};

    if (piece_info.type == cbn::Piece_type::Rook || piece_info.type == cbn::Piece_type::Queen)
    {
        // create pairs of all directions for rooks move
        auto offsets = lmn::generate_mixes(cbn::ROOK_OFFSET_X, cbn::ROOK_OFFSET_Y);
        auto extension = lmn::generate_mixes(cbn::ROOK_OFFSET_Y, cbn::ROOK_OFFSET_X);
        offsets.insert(offsets.begin(), extension.begin(), extension.end());

        for (const auto& [offset_x, offset_y] : offsets)
        {
            append_legalmoves_rook(legal_moves, piece_info, location, offset_x, offset_y);
        }
    }

    if (piece_info.type == cbn::Piece_type::Bishop || piece_info.type == cbn::Piece_type::Queen)
    {
        // create pairs of all directions for bishops move
        auto offsets = lmn::generate_mixes(cbn::BISHOP_OFFSET, cbn::BISHOP_OFFSET);
        for (const auto& [offset_x, offset_y] : offsets)
        {
            append_bishop_diagonal(legal_moves, piece_info, location, offset_x, offset_y);
        }
    }

    else if (piece_info.type == cbn::Piece_type::Pawn)
    {
        append_legalmoves_pawn(legal_moves, piece_info, location, cbn::PAWN_OFFSET_X, cbn::PAWN_OFFSET_Y);
    }

    else if (piece_info.type == cbn::Piece_type::Knight)
    {
        // create pairs of all offsets for knight moves
        auto offsets = lmn::generate_mixes(cbn::KNIGHT_OFFSET_LONG_SIDE, cbn::KNIGHT_OFFSET_SHORT_SIDE);
        auto extension = lmn::generate_mixes(cbn::KNIGHT_OFFSET_SHORT_SIDE, cbn::KNIGHT_OFFSET_LONG_SIDE);
        offsets.insert(offsets.end(), extension.begin(), extension.end());

        for (const auto& [offset_x, offset_y] : offsets)
            append_knight_move(legal_moves, piece_info, location, offset_x, offset_y);
    }

    else if (piece_info.type == cbn::Piece_type::King)
    {
        // create pairs of all possible offsets for kings move
        auto offsets = lmn::generate_mixes(cbn::KING_OFFSET_DIAGONAL, cbn::KING_OFFSET_DIAGONAL);
        auto extension = lmn::generate_mixes(cbn::KING_OFFSET_DIAGONAL, cbn::KING_OFFSET_CROSSWAYS);
        offsets.insert(offsets.end(), extension.begin(), extension.end());
        extension = lmn::generate_mixes(cbn::KING_OFFSET_CROSSWAYS, cbn::KING_OFFSET_DIAGONAL);
        offsets.insert(offsets.end(), extension.begin(), extension.end());

        for (const auto& [offset_x, offset_y] : offsets)
            append_legalmoves_king(legal_moves, piece_info, location, offset_x, offset_y);
    }

    std::sort(legal_moves.begin(), legal_moves.end());

    return legal_moves;
}

cbn::container_type<std::pair<int,int>, cbn::allocator_type<std::pair<int,int>>> lmn::generate_mixes(int i1, int i2)
// return container of pairs of all possible mixes of i1 and i2
// including negative and positive values
// all values in mix are unique
{
    cbn::container_type<std::pair<int,int>, cbn::allocator_type<std::pair<int,int>>> mix;

    for (int __ = 0; __ < 2; ++__)
    {
        for (int ___ = 0; ___ < 2; ++___)
        {
            std::pair<int,int> pair {i1, i2};

            // if not already in mix
            if (std::find(mix.begin(), mix.end(), pair) == mix.end())
                mix.push_back(pair);
            i2 = -i2;
        }
        i1 = -i1;
    }

    return mix;
}

bool lmn::Legalmoves::is_enemy(const cbn::ChessCoordinate& l1, const cbn::ChessCoordinate& l2)
{
    cbn::Piece_data d1 = get_piece_info(l1);
    cbn::Piece_data d2 = get_piece_info(l2);

    return d1.color != d2.color;
}

/*************************Functions requiring Legalmoves and Chessboard****************************/

void cbn::ChessBoard::move(const cbn::ChessNotation& movement)
// move a piece on the chess board from movement.x to movement.y
{
    lmn::Legalmoves legal{*this};
    
    auto legal_moves = legal(movement.from);

    if (move_is_legal(legal_moves, movement))
    {
        cbn::value_type& square_ref = board[movement.from.integer][movement.from.character];
        board[movement.to.integer][movement.to.character] = square_ref;
        square_ref = EMPTY_SQUARE;
    }
    else 
        throw cbn::IllegalMove;
}