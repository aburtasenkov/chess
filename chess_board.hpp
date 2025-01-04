#include <vector>
#include <algorithm>
#include "chess_notation.hpp"
#include "chess_board_constants.hpp"

namespace cbn
{
    using namespace chess_notation;
    using namespace chess_pieces;
    using coordinate_container = container_type<ChessCoordinate, allocator_type<ChessCoordinate>>;
    using notation_container = container_type<ChessNotation, allocator_type<ChessNotation>>;

    class ChessBoard{

        public:
            friend std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

            void restore();

            Piece& operator[](const ChessCoordinate& location);

            const Piece& operator[](const ChessCoordinate& location) const;

            ChessNotation& last_move();
            const ChessNotation& last_move() const;

            bool piece_was_moved(const ChessCoordinate& x) const;

            void move(const coordinate_container& move_list, const ChessNotation& xy);

            bool is_castle(const cbn::ChessNotation& move) const;

            bool is_enemy(const cbn::ChessCoordinate& l1, const cbn::ChessCoordinate& l2) const;

            bool passant_is_legal(const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& square) const;

            const Piece_color& colors_turn() const;

            bool is_checked(const Piece_color& color);

            bool move_is_unchecking(const ChessNotation& move);

            bool is_game_over(const Piece_color& color);

            notation_container& get_history();

        private:
            void move_piece(const ChessNotation& move);

            bn::Board<container_type, Piece, allocator_type> board{DEFAULT_CHESS_BOARD};
            notation_container move_history;
            Piece_color moving_turn{Piece_color::White};
    };

    class TemporalMove{
        public:
            TemporalMove(ChessBoard& b, const ChessNotation& m)
                :board(b), move(m), temp_to(board[move.to])
            {
                // move pieces
                board[move.to] = board[move.from];
                board[move.from] = EMPTY_SQUARE;
            }
            ~TemporalMove()
            {
                // restore previous state
                board[move.from] = board[move.to];
                board[move.to] = temp_to;
            }
        private:
            ChessBoard& board;
            const ChessNotation& move;
            const Piece temp_to{};        
    };

    /****************************************************Function declaration************************************************************************************/

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);
    
    // return true if square value is empty
    bool is_empty(const Piece& val);

    // return true if movement.from is in move_list --> the move is legal
    bool move_is_legal(const coordinate_container& move_list, const ChessNotation& movement);

    cbn::coordinate_container coordinates_between_xy(cbn::ChessCoordinate x, const cbn::ChessCoordinate& y);

    bool coordinates_are_empty(const cbn::ChessBoard& board, const cbn::coordinate_container& c);
}

/**************************************************************************************Function definition*******************************************************************/

cbn::notation_container& cbn::ChessBoard::get_history()
{
    return move_history;
}

std::ostream& cbn::operator<<(std::ostream& os, const cbn::ChessBoard& cb)
{
    for (int i = 0; i < cb.board.size(); ++i)
    {
        const auto& row = cb.board[i];

        os << i + 1 << ' '; // output number of rank
        
        // output piece by piece of a rank
        for (const auto& piece : row)
            os << piece << ' ';

        os << "\n";
    }
    for (int i = 0; i < CHAR_COORDINATE_RANK.size(); ++i)
        os << CHAR_COORDINATE_RANK[i] << ' '; 
    return os << "\n";
}

void cbn::ChessBoard::restore()
{
    board = DEFAULT_CHESS_BOARD;
    moving_turn = Piece_color::White;
}

cbn::Piece& cbn::ChessBoard::operator[](const cbn::ChessCoordinate& location)
{
    return board[location.integer][location.character];
}

const cbn::Piece& cbn::ChessBoard::operator[](const cbn::ChessCoordinate& location) const
{
    return board[location.integer][location.character];
}

bool cbn::is_empty(const Piece& square)
{
    return square.type == EMPTY_SQUARE.type;
}

bool cbn::move_is_legal(const cbn::coordinate_container& move_list, const cbn::ChessNotation& movement)
{
    return (std::find(move_list.begin(), move_list.end(), movement.to) != move_list.end());
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
        if (move.to == x)
            return true;
    }
    return false;
}

const cbn::Piece_color& cbn::ChessBoard::colors_turn() const
{
    return moving_turn;
}

/****************************************************************************************************************************************/

namespace lmn
{
    class Legalmoves
    {
        public:
            Legalmoves(cbn::ChessBoard& b)
                :board(b)   {   }
            
            // calculate all legal moves for piece at location
            const cbn::coordinate_container& get_legal_moves(const cbn::ChessCoordinate& location);
            const cbn::coordinate_container& get_potential_moves(const cbn::ChessCoordinate& location);
        
        private:
            void append_legalmoves_pawn(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_legalmoves_pawn_eating(const cbn::ChessCoordinate& location, std::initializer_list<cbn::ChessCoordinate> list);
            void append_en_passant(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_legalmoves_rook(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_knight_move(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_bishop_diagonal(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            
            void append_legalmoves_king(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y);
            void append_castling(const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& rook_location);
            
            cbn::ChessBoard& board;
            cbn::coordinate_container move_list;
    };

    cbn::container_type<std::pair<int,int>, cbn::allocator_type<std::pair<int,int>>> generate_mixes(int i1, int i2);
};

/*******************************************************************Function definition*********************************************************************/

bool cbn::ChessBoard::is_castle(const cbn::ChessNotation& move) const
// return if the move is a castle or not
{
    const cbn::Piece& piece = operator[](move.from);

    // moving piece is king and moves 2 squares
    if (piece.type == cbn::Piece_type::King && abs(move.from.character - move.to.character) == cbn::CASTLE_OFFSET)
        return true;
    return false;
}


bool cbn::coordinates_are_empty(const cbn::ChessBoard& board, const cbn::coordinate_container& c)
{
    for (const auto& x : c)
    {
        if (!cbn::is_empty(board[x]))
            return false;
    }
    return true;
}

bool cbn::ChessBoard::passant_is_legal(const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& square) const
// return true if an en passant move is legal
// location is the current piece, square is the piece left or right of it
{
    // check so i can call get_piece_data
    if (is_empty(operator[](square)))
        return false;

    if (!is_enemy(location, square))
        return false;

    if (operator[](location).type != operator[](square).type) // need to be of same type
        return false;

    // same character rank and move difference is 2
    if (last_move().from.character != last_move().to.character)
        return false;
    if (abs(last_move().from.integer - last_move().to.integer) != 2)
        return false;
    
    // need to be close to each other
    if (abs(last_move().to.character - location.character) != 1)
        return false;

    return true;
}

void lmn::Legalmoves::append_legalmoves_pawn(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
// append move_list with possible moves for pawns
{
    cbn::ChessCoordinate front, front_2;
    int starting_row;

    if (board[location].color == cbn::Piece_color::White)
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
        move_list.push_back(front);
        
        if (location.integer == starting_row && cbn::is_empty(board[front_2]))    // pawns on starting row can move 2 squares
            move_list.push_back(front_2);
    }

    // Check for eating other pieces
    append_legalmoves_pawn_eating(location, 
                            {
                            front + cbn::ChessCoordinate{-offset_y, 0}, 
                            front + cbn::ChessCoordinate{offset_y, 0}
                            });
    append_en_passant(location, offset_x, offset_y);

    return;
}

void lmn::Legalmoves::append_legalmoves_pawn_eating(const cbn::ChessCoordinate& location, std::initializer_list<cbn::ChessCoordinate> list)
{
    for (const auto& square : list)
    {
        if (!square.is_valid())
            continue;

        if (!cbn::is_empty(board[square]) && board.is_enemy(location, square))
            move_list.push_back(square);
    }
    return;
}

void lmn::Legalmoves::append_en_passant(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate en_passant_1 = location + cbn::ChessCoordinate{-offset_y, 0};
    cbn::ChessCoordinate en_passant_2 = location + cbn::ChessCoordinate{offset_y, 0};

    // check for en passants
    for (const auto& square : {en_passant_1, en_passant_2})
    {
        if (!square.is_valid())
            continue;

        if (!board.passant_is_legal(location, square))
            continue;

        cbn::ChessCoordinate en_passant_coordinate;

        const cbn::ChessNotation& last_move = board.last_move();

        if (board[location].color == cbn::Piece_color::White)
            en_passant_coordinate = square + cbn::ChessCoordinate{0, -offset_y};  // move up from last move
        else
            en_passant_coordinate = square + cbn::ChessCoordinate{0, offset_y};   // move down from last move

        move_list.push_back(en_passant_coordinate);
    }

    return;
}

void lmn::Legalmoves::append_legalmoves_rook(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate current = location + cbn::ChessCoordinate{offset_x, offset_y};

    while (current.is_valid())
    {
        // can go to empty coordinates
        if (cbn::is_empty(board[current]))
            move_list.push_back(current);

        // but if its an enemy -> break the loop
        else
        {
            if (board.is_enemy(location, current))
                move_list.push_back(current);
            break;
        }

        current += cbn::ChessCoordinate{offset_x, offset_y};
    }

    return;
}

void lmn::Legalmoves::append_knight_move(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate current = location + cbn::ChessCoordinate{offset_x, offset_y};

    if (!current.is_valid())
        return;

    if (cbn::is_empty(board[current]) || board.is_enemy(location, current))
        move_list.push_back(current);

    return;
}

void lmn::Legalmoves::append_bishop_diagonal(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate diagonal = location + cbn::ChessCoordinate{offset_x, offset_y};

    while (diagonal.is_valid())
    {
        // can go to empty coordinates
        if (cbn::is_empty(board[diagonal]))
            move_list.push_back(diagonal);

        // but if its an enemy -> break the loop
        else
        {
            if (board.is_enemy(location, diagonal))
                move_list.push_back(diagonal);
            break;
        }

        diagonal += cbn::ChessCoordinate{offset_x, offset_y};
    }

    return;
}

void lmn::Legalmoves::append_legalmoves_king(const cbn::ChessCoordinate& location, const int offset_x, const int offset_y)
{
    cbn::ChessCoordinate current = location + cbn::ChessCoordinate{offset_x, offset_y};
    if (!current.is_valid())
        return;

    // append if the square is empty or is an enemy
    if (cbn::is_empty(board[current]) || board.is_enemy(location, current))
        move_list.push_back(current);

    return;
}

void lmn::Legalmoves::append_castling(const cbn::ChessCoordinate& location, const cbn::ChessCoordinate& rook_location)
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
    
    if (!castle_location.is_valid())
        return;

    // no pieces between king and rook
    if (cbn::coordinates_are_empty(board, coordinates_between_pieces))
        move_list.push_back(castle_location);

    return;
}

const cbn::coordinate_container& lmn::Legalmoves::get_legal_moves(const cbn::ChessCoordinate& location)
{
    move_list = cbn::coordinate_container{};
    get_potential_moves(location);

    for (auto iter = move_list.begin(); iter != move_list.end();)
    {
        cbn::ChessNotation move{location, *iter};
        cbn::TemporalMove _{board, move};
        if (board.is_checked(board[*iter].color))
            move_list.erase(iter);
        else
            ++iter;
    }

    std::sort(move_list.begin(), move_list.end());
    return move_list;
}

const cbn::coordinate_container& lmn::Legalmoves::get_potential_moves(const cbn::ChessCoordinate& location)
// return a container containing all legal moves for kind located at location
{
    if (board[location].type == cbn::Piece_type::Rook || board[location].type == cbn::Piece_type::Queen)
    {
        // create pairs of all directions for rooks move
        auto offsets = lmn::generate_mixes(cbn::ROOK_OFFSET_X, cbn::ROOK_OFFSET_Y);
        auto extension = lmn::generate_mixes(cbn::ROOK_OFFSET_Y, cbn::ROOK_OFFSET_X);
        offsets.insert(offsets.begin(), extension.begin(), extension.end());

        for (const auto& [offset_x, offset_y] : offsets)
        {
            append_legalmoves_rook(location, offset_x, offset_y);
        }
    }

    if (board[location].type == cbn::Piece_type::Bishop || board[location].type == cbn::Piece_type::Queen)
    {
        // create pairs of all directions for bishops move
        auto offsets = lmn::generate_mixes(cbn::BISHOP_OFFSET, cbn::BISHOP_OFFSET);
        for (const auto& [offset_x, offset_y] : offsets)
        {
            append_bishop_diagonal(location, offset_x, offset_y);
        }
    }

    else if (board[location].type == cbn::Piece_type::Pawn)
    {
        append_legalmoves_pawn(location, cbn::PAWN_OFFSET_X, cbn::PAWN_OFFSET_Y);
    }

    else if (board[location].type == cbn::Piece_type::Knight)
    {
        // create pairs of all offsets for knight moves
        auto offsets = lmn::generate_mixes(cbn::KNIGHT_OFFSET_LONG_SIDE, cbn::KNIGHT_OFFSET_SHORT_SIDE);
        auto extension = lmn::generate_mixes(cbn::KNIGHT_OFFSET_SHORT_SIDE, cbn::KNIGHT_OFFSET_LONG_SIDE);
        offsets.insert(offsets.end(), extension.begin(), extension.end());

        for (const auto& [offset_x, offset_y] : offsets)
            append_knight_move(location, offset_x, offset_y);
    }

    else if (board[location].type == cbn::Piece_type::King)
    {
        // create pairs of all possible offsets for kings move
        auto offsets = lmn::generate_mixes(cbn::KING_OFFSET_DIAGONAL, cbn::KING_OFFSET_DIAGONAL);
        auto extension = lmn::generate_mixes(cbn::KING_OFFSET_DIAGONAL, cbn::KING_OFFSET_CROSSWAYS);
        offsets.insert(offsets.end(), extension.begin(), extension.end());
        extension = lmn::generate_mixes(cbn::KING_OFFSET_CROSSWAYS, cbn::KING_OFFSET_DIAGONAL);
        offsets.insert(offsets.end(), extension.begin(), extension.end());

        for (const auto& [offset_x, offset_y] : offsets)
            append_legalmoves_king(location, offset_x, offset_y);
        
        cbn::ChessCoordinate left_rook, right_rook;
        if (board[location].color == cbn::Piece_color::White)
        {
            left_rook = {cbn::LEFT_ROOK_CHARACTER, cbn::WHITE_BACK_RANK};
            right_rook = {cbn::RIGHT_ROOK_CHARACTER, cbn::WHITE_BACK_RANK};
        }
        else
        {
            left_rook = {cbn::LEFT_ROOK_CHARACTER, cbn::BLACK_BACK_RANK};
            right_rook = {cbn::RIGHT_ROOK_CHARACTER, cbn::BLACK_BACK_RANK};
        }

        append_castling(location, left_rook);
        append_castling(location, right_rook);
    }

    return move_list;
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

bool cbn::ChessBoard::is_enemy(const cbn::ChessCoordinate& l1, const cbn::ChessCoordinate& l2) const
{
    return operator[](l1).color != operator[](l2).color;
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

void cbn::ChessBoard::move(const cbn::coordinate_container& move_list, const cbn::ChessNotation& move)
// move a piece on the chess board from move.x to move.y
{
    if (move_is_legal(move_list, move))
    {
        // if en pessant
        if (!move_history.empty())
        {
            auto last = last_move();    // need last move to check if en passant is legal
            if (passant_is_legal(move.from, last.to))
            {
                // if pawn moves diagonally
                if (last.to.character == move.to.character)
                    operator[](last.to) = EMPTY_SQUARE; // remove the last moved piece
            }
        }

        if (is_castle(move))
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

        // enemy is at move now
        moving_turn = enemy_color.at(moving_turn);
    }
    else 
        throw cbn::IllegalMoveError;
}

void cbn::ChessBoard::move_piece(const ChessNotation& move)
{
    operator[](move.to) = operator[](move.from);
    operator[](move.from)= EMPTY_SQUARE;

    move_history.push_back(move);
}

bool cbn::ChessBoard::is_checked(const Piece_color& color)
// return if color is checked
{
    lmn::Legalmoves legal(*this);

    // iterate all pieces
    for (int row_i = 0; row_i < board.size(); ++row_i)
    {
        const auto& row = board[row_i];
        for (int piece_i = 0; piece_i < row.size(); ++piece_i)
        {
            ChessCoordinate current{piece_i, row_i};
            if (operator[](current).color == color)   // if not enemy to current moving color
                continue;

            const auto coords = legal.get_potential_moves(current);
            
            // iterate over legal moves of current square
            for (const auto& coord : coords)
            {
                if (operator[](coord).type == Piece_type::King && operator[](coord).color == color)
                    return true;
            }
        }
    }
    return false;
}

bool cbn::ChessBoard::move_is_unchecking(const cbn::ChessNotation& move)
{
    bool output_value = false;
    TemporalMove temporal(*this, move);

    if (!is_checked(moving_turn))
        output_value = true;

    return output_value;
}

bool cbn::ChessBoard::is_game_over(const cbn::Piece_color& color)
// return if color has no legal moevs to do
{
    lmn::Legalmoves legal(*this);

    // iterate all pieces
    for (int row_i = 0; row_i < board.size(); ++row_i)
    {
        const auto& row = board[row_i];
        for (int piece_i = 0; piece_i < row.size(); ++piece_i)
        {
            ChessCoordinate current{piece_i, row_i};
            const auto& piece = operator[](current);

            // check if enemy color
            if (piece.color != color) 
                continue;

            const auto move_list = legal.get_legal_moves(current);
            
            if (move_list.size() > 0)
                return false;
        }
    }
    return true;
}

