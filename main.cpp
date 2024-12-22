#include "chess_board.hpp"
using namespace cbn;
using namespace lmn;

ChessBoard board;
Legalmoves legal{board};

template <typename T>
std::ostream& operator<<(std::ostream& os,std::vector<T>& v)
{
    for (auto& x : v)
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

        auto legal_moves = legal(from);
        std::cout << "List of all legal moves: " << legal_moves << "\n";

        std::cin >> to;

        board.move(ChessNotation{from, to});
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << "\n";
    }
}
