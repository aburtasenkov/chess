#include "chess_board.hpp"
using namespace cbn;
using namespace lmn;

ChessBoard board;
Legalmoves legal{board};

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

int main()
{
    ChessNotation move;

    while (true)
    try {
        std::cout << board;

        std::cin >> move;

        if (!(board[move.from].color == board.colors_turn()))
            throw BadSequenceError;

        const auto& move_list = legal(move.from);

        if (board.is_checked(board.colors_turn()))
        {
            if (!board.move_is_unchecking(move))
                throw KingIsCheckedError;
        }

        std::cout << "List of all legal moves: " << move_list << "\n";

        board.move(move_list, move);

        if (board.is_checked(board.colors_turn()))
        {
            if (board.is_checkmated(board.colors_turn()))
                {
                    std::cout << "CHECKMATE\n";
                    board.restore();
                }
            else
                std::cout << "Check\n";
        }
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << "\n";
    }
}
