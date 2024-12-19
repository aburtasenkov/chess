#include "chess_board.hpp"
using namespace cbn;

ChessBoard board;

int main()
{
    while (true)
    {
        std::cout << board;
        ChessCoordinate x, y;
        std::cin >> x >> y;
        board.move({x,y});
    }
    return 0;
}

