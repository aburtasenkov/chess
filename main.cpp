#include "chess_board.hpp"
using namespace cbn;
using namespace lmn;

ChessBoard board;

template <typename T>
std::ostream& operator<<(std::ostream& os,std::vector<T>& v)
{
    for (auto& x : v)
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

        board.move(move);
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << "\n";
    }
}
