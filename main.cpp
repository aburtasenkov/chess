#include "chess_board.hpp"
using namespace cbn;
using namespace lmn;

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

ChessBoard board;
Legalmoves legal{board};

int main()
{
    while (true)
    try {
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
