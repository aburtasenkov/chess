#include <vector>
#include <algorithm>
#include "chess_notation.hpp"
#include "chess_board_constants.hpp"

namespace cbn
{
    using namespace chess_notation;
    using namespace chess_board_constants;
    using coordinate_container = container_type<ChessCoordinate, allocator_type<ChessCoordinate>>;
    using notation_container = container_type<ChessNotation, allocator_type<ChessNotation>>;

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

            value_type& operator[](const ChessCoordinate& location);

            const value_type& operator[](const ChessCoordinate& location) const;

            ChessNotation& last_move();
            const ChessNotation& last_move() const;

            void move(const ChessNotation& xy);

            bool piece_was_moved(const ChessCoordinate& x) const;

        private:
            void move_piece(const ChessNotation& move);

            bn::Board<container_type, value_type, allocator_type> board{DEFAULT_CHESS_BOARD};
            notation_container move_history;
    };

    /****************************************************Function declaration************************************************************************************/

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

    // return true if square value is empty
    bool is_empty(const value_type& val);

    // return true if movement.from is in legal_moves --> the move is legal
    bool move_is_legal(const coordinate_container& legal_moves, const ChessNotation& movement);

    cbn::coordinate_container coordinates_between_xy(cbn::ChessCoordinate x, const cbn::ChessCoordinate& y);
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

cbn::ChessNotation& cbn::ChessBoard::last_move()
{
    return move_history.back();
}

const cbn::ChessNotation& cbn::ChessBoard::last_move() const
{
    return move_history.back();
}

bool cbn::ChessBoard::piece_was_moved(const cbn::ChessCoordinate& x) const
{
    for (const auto& move : move_history)
    {
        if (move.from == x)
            return true;
    }
    return false;
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

            const cbn::Piece_data get_piece_data(const cbn::ChessCoordinate& location) const;

            bool is_en_passant(const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& square) const;

            bool is_enemy(const cbn::ChessCoordinate& l1, const cbn::ChessCoordinate& l2) const;

            bool is_castle(const cbn::ChessNotation& move);
        
        private:
            const cbn::ChessBoard& board;

            void append_legalmoves_pawn(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_legalmoves_pawn_eating(cbn::coordinate_container& legal_moves, const cbn::ChessCoordinate& location, std::initializer_list<cbn::ChessCoordinate> list);
            void append_en_passant(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_legalmoves_rook(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_knight_move(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_bishop_diagonal(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_legalmoves_king(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_castling(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& rook_location);
    };

    cbn::container_type<std::pair<int,int>, cbn::allocator_type<std::pair<int,int>>> generate_mixes(int i1, int i2);

    bool coordinates_are_empty(const cbn::ChessBoard& board, const cbn::coordinate_container& c);
};

/*******************************************************************Function definition*********************************************************************/

bool lmn::Legalmoves::is_castle(const cbn::ChessNotation& move)
// return if the move is a castle or not
{
    const cbn::Piece_data piece_info = get_piece_data(move.from);

    // moving piece is king and moves 2 squares
    if (piece_info.type == cbn::Piece_type::King && abs(move.from.character - move.to.character) == cbn::CASTLE_OFFSET)
        return true;
    return false;
}


bool lmn::coordinates_are_empty(const cbn::ChessBoard& board, const cbn::coordinate_container& c)
{
    for (const auto& x : c)
    {
        if (!cbn::is_empty(board[x]))
            return false;
    }
    return true;
}

bool lmn::Legalmoves::is_en_passant(const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& square) const
{
    if (board[square] == cbn::EMPTY_SQUARE)
        return false;

    if (!is_enemy(location, square))
        return false;

    const cbn::Piece_data piece_info = get_piece_data(location);
    const cbn::Piece_data square_info = get_piece_data(square);

    if (piece_info.type != square_info.type) // need to be of same type
        return false;
    
    const cbn::ChessNotation& last_move = board.last_move();

    // same character rank and move difference is 2
    if (!(last_move.from.character == last_move.to.character && abs(last_move.from.integer - last_move.to.integer) == 2))
        return false;

    return true;
}

const cbn::Piece_data lmn::Legalmoves::get_piece_data(const cbn::ChessCoordinate& location) const
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
    cbn::ChessCoordinate front, front_2;
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

    const cbn::coordinate_container potential_squares = 
    {
        front + cbn::ChessCoordinate{-offset_y, 0}, 
        front + cbn::ChessCoordinate{offset_y, 0}
    };

    if (cbn::is_empty(board[front]))
    {
        legal_moves.push_back(front);
        
        if (location.integer == starting_row && cbn::is_empty(board[front_2]))    // pawns on starting row can move 2 squares
            legal_moves.push_back(front_2);
    }

    // Check for eating other pieces
    append_legalmoves_pawn_eating(legal_moves, location, 
                            {
                            front + cbn::ChessCoordinate{-offset_y, 0}, 
                            front + cbn::ChessCoordinate{offset_y, 0}
                            });
    append_en_passant(legal_moves, piece_info, location, offset_x, offset_y);

    return;
}

void lmn::Legalmoves::append_legalmoves_pawn_eating(cbn::coordinate_container& legal_moves, const cbn::ChessCoordinate& location, std::initializer_list<cbn::ChessCoordinate> list)
{
    for (const auto& square : list)
    {
        if (!square.is_valid())
            continue;

        if (!cbn::is_empty(board[square]) && is_enemy(location, square))
            legal_moves.push_back(square);
    }
    return;
}

void lmn::Legalmoves::append_en_passant(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate en_passant_1 = location + cbn::ChessCoordinate{-offset_y, 0};
    cbn::ChessCoordinate en_passant_2 = location + cbn::ChessCoordinate{offset_y, 0};

    // check for en passants
    for (const auto& square : {en_passant_1, en_passant_2})
    {
        if (!square.is_valid())
            continue;

        if (!is_en_passant(location, square))
            continue;

        cbn::ChessCoordinate en_passant_coordinate;

        const cbn::ChessNotation& last_move = board.last_move();

        if (piece_info.color == cbn::Piece_color::White)
            en_passant_coordinate = last_move.to + cbn::ChessCoordinate{0, -offset_y};  // move up from last move
        else
            en_passant_coordinate = last_move.to + cbn::ChessCoordinate{0, offset_y};   // move down from last move

        legal_moves.push_back(en_passant_coordinate);
    }

    return;
}

void lmn::Legalmoves::append_legalmoves_rook(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate current = location + cbn::ChessCoordinate{offset_x, offset_y};

    while (current.is_valid())
    {
        // can go to empty coordinates
        if (cbn::is_empty(board[current]))
            legal_moves.push_back(current);

        // but if its an enemy -> break the loop
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

    if (cbn::is_empty(board[current]) || is_enemy(location, current))
        legal_moves.push_back(current);

    return;
}

void lmn::Legalmoves::append_bishop_diagonal(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate diagonal = location + cbn::ChessCoordinate{offset_x, offset_y};

    while (diagonal.is_valid())
    {
        // can go to empty coordinates
        if (cbn::is_empty(board[diagonal]))
            legal_moves.push_back(diagonal);

        // but if its an enemy -> break the loop
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
    if (!current.is_valid())
        return;

    // append if the square is empty or is an enemy
    if (cbn::is_empty(board[current]) || is_enemy(location, current))
        legal_moves.push_back(current);

    return;
}

void lmn::Legalmoves::append_castling(cbn::coordinate_container& legal_moves, const cbn::Piece_data& piece_info, const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& rook_location)
// location is the king coordinate
{
    // Both pieces were not moved yet
    if (board.piece_was_moved(location) || board.piece_was_moved(rook_location))
        return;

    cbn::coordinate_container coordinates_between_pieces = cbn::coordinates_between_xy(rook_location, location);

    cbn::ChessCoordinate castle_location;

    if (rook_location.character == cbn::LEFT_ROOK_CHARACTER)
        castle_location = location - cbn::ChessCoordinate{cbn::CASTLE_OFFSET, 0};
    else
        castle_location = location + cbn::ChessCoordinate{cbn::CASTLE_OFFSET, 0};

    // no pieces between king and rook
    if (coordinates_are_empty(board, coordinates_between_pieces))
        legal_moves.push_back(castle_location);

    return;
}
     
const cbn::coordinate_container lmn::Legalmoves::operator()(const cbn::ChessCoordinate& location)
// return a container containing all legal moves for kind located at location
{
    const cbn::Piece_data piece_info = get_piece_data(location);

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
        

        cbn::ChessCoordinate left_rook, right_rook;
        if (piece_info.color == cbn::Piece_color::White)
        {
            left_rook = {cbn::LEFT_ROOK_CHARACTER, cbn::WHITE_BACK_RANK};
            right_rook = {cbn::RIGHT_ROOK_CHARACTER, cbn::WHITE_BACK_RANK};
        }
        else
        {
            left_rook = {cbn::LEFT_ROOK_CHARACTER, cbn::BLACK_BACK_RANK};
            right_rook = {cbn::RIGHT_ROOK_CHARACTER, cbn::BLACK_BACK_RANK};
        }

        append_castling(legal_moves, piece_info, location, left_rook);
        append_castling(legal_moves, piece_info, location, right_rook);
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

bool lmn::Legalmoves::is_enemy(const cbn::ChessCoordinate& l1, const cbn::ChessCoordinate& l2) const
{
    const cbn::Piece_data d1 = get_piece_data(l1);
    const cbn::Piece_data d2 = get_piece_data(l2);

    return d1.color != d2.color;
}

cbn::coordinate_container cbn::coordinates_between_xy(cbn::ChessCoordinate x, const cbn::ChessCoordinate& y)
// return container all coordinates between x and y (without including them)
{
    cbn::coordinate_container coordinates{};

    while (x != y)
    {
        // move 1 closer on character axis
        if (x.character > y.character)
            x -= cbn::ChessCoordinate{1,0};
        else if (x.character < y.character)
            x += cbn::ChessCoordinate{1,0};
        
        // move 1 closer on 
        if (x.integer > y.integer)
            x -= cbn::ChessCoordinate{0,1};
        else if (x.integer < y.integer)
            x += cbn::ChessCoordinate{0,1};

        // otherwise equivalent of y will be added to the container
        if (x != y)
            coordinates.push_back(x);  
    }

    return coordinates;
}

/*************************Functions requiring Legalmoves and Chessboard****************************/

void cbn::ChessBoard::move(const cbn::ChessNotation& move)
// move a piece on the chess board from move.x to move.y
{
    // get all legal moves for move.from    
    lmn::Legalmoves legal{*this};
    auto legal_moves = legal(move.from);

    if (move_is_legal(legal_moves, move))
    {
        // if en pessant
        if (!move_history.empty())
        {
            auto last = last_move();    // need last move to check if en passant is legal
            if (legal.is_en_passant(move.from, last.to))
            {
                // if pawn moves diagonally
                if (last.to.character == move.to.character)
                    operator[](last.to) = EMPTY_SQUARE; // remove the last moved piece
            }
        }

        if (legal.is_castle(move))
        {
            // move.to.ingeter and move.from.integer are same as the rank doesnt change for the piece

            cbn::ChessNotation rook_move;
            
            // if left castle
            if (move.to.character == cbn::LEFT_CASTLE_CHARACTER)
            {
                rook_move =
                {
                    {cbn::LEFT_ROOK_CHARACTER, move.to.integer},
                    {cbn::LEFT_CASTLE_CHARACTER + cbn::CASTLE_OFFSET / 2, move.to.integer}
                };
            }
            else    // right castle
            {
                rook_move =
                {
                    {cbn::RIGHT_ROOK_CHARACTER, move.to.integer},
                    {cbn::RIGHT_CASTLE_CHARACTER - cbn::CASTLE_OFFSET / 2, move.to.integer}
                };
            }
            move_piece(rook_move);
        }

        move_piece(move);
    }
    else 
        throw cbn::IllegalMove;
}

void cbn::ChessBoard::move_piece(const ChessNotation& move)
{
    operator[](move.to) = operator[](move.from);
    operator[](move.from)= EMPTY_SQUARE;

    move_history.push_back(move);
}