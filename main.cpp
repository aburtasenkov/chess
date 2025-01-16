#include <iostream>
#include <vector>

#include "chess_bot.hpp"
#include "chess_board.hpp"

using namespace cbn;
using namespace lmn;
using namespace cbot;

template <typename T, typename TT>
std::ostream& operator<<(std::ostream& os, const std::pair<T,TT>& pair)
{
    os << '{' << pair.first << ", " << pair.second << '}';
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    for (const auto& x : v)
        os << x << " ";
    return os;
}

ChessBoard board{};
Legalmoves legal{board};
Engine bot{};

const int COMPUTATION_DEPTH = 4;

int main()
{
    while (true)
    try {
        // Bot is moving
        if (board.colors_turn() == Piece_color::Black)
        {
            const auto notation = bot.best_notation(board, COMPUTATION_DEPTH);
            const auto& move_list = legal.get_legal_moves(notation.from);
            board.move(move_list, notation);
            continue;
        }
        
        ChessCoordinate from, to;
        
        std::cout << board;

        std::cin >> from;

        const auto& move_list = legal.get_legal_moves(from);

        std::cout << "List of all legal moves: " << move_list << "\n";

        std::cin >> to;

        ChessNotation move{from, to};

        if (board[move.from].color != board.colors_turn())
            throw BadSequenceError;

        if (board.is_checked(board.colors_turn()))
        {
            if (!board.move_is_unchecking(move))
            {
                throw KingIsCheckedError;
            }
        }

        // const auto best_move = bot.best_move(board, move.from);

        board.move(move_list, move);
        
        // if (move != best_move)
        //     std::cout << "Better move would be - " << best_move << "\n";

        std::cout << "New Score is - " << cbot::board_score(board, board[move.to].color) << "\n";

        if (board.is_checked(board.colors_turn()))
        {
            if (board.is_game_over(board.colors_turn()))
                {
                    std::cout << "CHECKMATE\n";
                    board.restore();
                }
            else
                std::cout << "Check\n";
        }

        if (board.is_game_over(board.colors_turn()))
            std::cout << "ITS DRAW\n";

    }
    catch (Exception& e)
    {
        std::cerr << e.what() << "\n";
    }
}
