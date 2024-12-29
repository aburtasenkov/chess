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
    ChessCoordinate from, to;

    while (true)
    try {
        std::cout << board;

        std::cin >> from;

        const auto& move_list = legal(from);

        std::cout << "List of all legal moves: " << move_list << "\n";

        std::cin >> to;

        board.move(move_list, ChessNotation{from, to});
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << "\n";
    }
}
