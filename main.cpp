#include "chess_board.hpp"
#include "chess_notation.hpp"
using namespace cbn;

ChessBoard board;

int main()
{
    ChessMove cm;
    std::cin >> cm;
    std::cout << cm << "\n";
    std::cout << board;
    return 0;
}

