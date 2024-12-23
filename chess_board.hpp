#include <vector>
#include <array>
#include <algorithm>
#include "chess_notation.hpp"
#include "chess_board_constants.hpp"

namespace cbn
{
    using namespace chess_notation;
    using namespace chess_board_constants;

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
    bool move_is_legal(const cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::ChessNotation& movement);
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

bool cbn::move_is_legal(const cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::ChessNotation& movement)
{
    return (std::find(legal_moves.begin(), legal_moves.end(), movement.to) != legal_moves.end());
}

/****************************************************************************************************************************************/

namespace lmn
{
    using namespace cbn;

    auto plus = [](int i, int ii) { return i + ii; };
    auto minus = [](int i, int ii) { return i - ii; };
    auto smaller_equal = [](int left, int right) { return left <= right; };
    auto bigger_equal = [](int left, int right) { return left >= right; };

    class Legalmoves
    {
        public:
            Legalmoves(const ChessBoard& b)
                :board(b)   {   }
            
            // calculate all legal moves for piece at location
            const container_type<ChessCoordinate, allocator_type<ChessCoordinate>> operator()(const ChessCoordinate& location);
            
        private:
            const ChessBoard& board;

            Piece_data get_piece_info(const ChessCoordinate& location);
            void append_legalmoves_pawn(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);
            void append_legalmoves_rook_horizontal(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);
            void append_legalmoves_rook_vertical(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);
            void append_legalmoves_knight(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);
            void append_legalmoves_bishop(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);
            void append_legalmoves_queen(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);
            void append_legalmoves_king(container_type<ChessCoordinate, allocator_type<ChessCoordinate>>& legal_moves, const Piece_data& piece_info, const ChessCoordinate& location);

    };
};

/*******************************************************************Function definition*********************************************************************/

cbn::Piece_data lmn::Legalmoves::get_piece_info(const cbn::ChessCoordinate& location)
{
    cbn::Piece_data piece_info;

    // make piece_info data
    {
    piece_info.value = board[location];

    if (is_empty(board[location]))
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

void lmn::Legalmoves::append_legalmoves_pawn(cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location)
// append legal_moves with possible moves for pawns
{
    // Check for en passant, TODO

    cbn::ChessCoordinate front = location - cbn::ChessCoordinate{0,1};    // coordinate in front of pawn
    cbn::ChessCoordinate front_2 = front - cbn::ChessCoordinate{0,1};     // coordinate 2 squares in front of pawn
    cbn::Piece_color curr_color = cbn::Piece_color::White;
    int starting_row = cbn::RANK_7_INDEX;

    // Do once for white, once for black
    for (int _ = 0; _ < 2; ++_)
    {
        if (piece_info.color == curr_color)
        {
            if (cbn::is_empty(board[front]))
            {
                legal_moves.push_back(front);
                
                if (location.integer == starting_row && cbn::is_empty(board[front_2]))    // pawns on starting row can move 2 squares
                    legal_moves.push_back(front_2);
            }
            return;
        }

        curr_color = cbn::Piece_color::Black;
        starting_row = cbn::RANK_2_INDEX;
        front = location + cbn::ChessCoordinate{0,1};
        front_2 = front + cbn::ChessCoordinate{0,1};
    }
    return;
}

void lmn::Legalmoves::append_legalmoves_rook_horizontal(cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location)
{
    typedef int (*arithmetic_func)(int, int);
    typedef bool (*boolean_func)(int,int);

    // right --> operations
    arithmetic_func operation = plus;
    boolean_func boolean = smaller_equal;
    int RANGE_REF = MAX_INDEX_HORIZONTAL;

    // do twice
    for (int _ = 0; _ < 2; ++_)
    {
        for (int shift = 1; boolean(operation(location.character, shift), RANGE_REF); ++shift)
        {
            cbn::ChessCoordinate current{operation(location.character, shift), location.integer};
            if (is_empty(board[current]))
                legal_moves.push_back(current);
            else 
                break;
        }

        // left <-- operations
        operation = minus;
        boolean = bigger_equal;
        RANGE_REF = MIN_INDEX_HORIZONTAL;
    }

    return;
}

void lmn::Legalmoves::append_legalmoves_rook_vertical(cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location)
{
    typedef int (*arithmetic_func)(int, int);
    typedef bool (*boolean_func)(int,int);

    // down operations
    arithmetic_func operation = plus;
    boolean_func boolean = smaller_equal;
    int RANGE_REF = MAX_INDEX_VERTICAL;

    // do twice
    for (int _ = 0; _ < 2; ++_)
    {
        for (int shift = 1; boolean(operation(location.integer, shift), RANGE_REF); ++shift)
        {
            cbn::ChessCoordinate current{location.character, operation(location.integer, shift)};
            if (is_empty(board[current]))
                legal_moves.push_back(current);
            else 
                break;
        }

        // up operations
        operation = minus;
        boolean = bigger_equal;
        RANGE_REF = MIN_INDEX_VERTICAL;
    }

    return;
}

void lmn::Legalmoves::append_legalmoves_knight(cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location)
{
    // todo
    return;
}

void lmn::Legalmoves::append_legalmoves_bishop(cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location)
{
    // todo
    return;
}

void lmn::Legalmoves::append_legalmoves_queen(cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location)
{
    // todo, use rook and bishop functions
    return;
}

void lmn::Legalmoves::append_legalmoves_king(cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>>& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location)
{
    // todo
    return;
}
     
const cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>> lmn::Legalmoves::operator()(const cbn::ChessCoordinate& location)
// return a container containing all legal moves for kind located at location
{
    cbn::Piece_data piece_info = get_piece_info(location);

    cbn::container_type<cbn::ChessCoordinate, cbn::allocator_type<cbn::ChessCoordinate>> legal_moves{};

    if (piece_info.type == cbn::Piece_type::Pawn)
        append_legalmoves_pawn(legal_moves, piece_info, location);

    else if (piece_info.type == cbn::Piece_type::Rook)
    {
        append_legalmoves_rook_horizontal(legal_moves, piece_info, location);
        append_legalmoves_rook_vertical(legal_moves, piece_info, location);
    }

    else if (piece_info.type == cbn::Piece_type::Knight)
    {
        append_legalmoves_knight(legal_moves, piece_info, location);
    }

    else if (piece_info.type == cbn::Piece_type::Bishop)
    {
        append_legalmoves_bishop(legal_moves, piece_info, location);
    }

    else if (piece_info.type == cbn::Piece_type::Queen)
    {
        append_legalmoves_queen(legal_moves, piece_info, location);
    }

    else if (piece_info.type == cbn::Piece_type::King)
    {
        append_legalmoves_king(legal_moves, piece_info, location);
    }

    return legal_moves;
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