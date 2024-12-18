#include "Chess_board.hpp"
using namespace chess_board_namespace;

ChessBoard board;

Board<std::vector, char> b{8,2, 'X'};

int main()
{
    std::cout << b << "\n";
    std::cout << board << "\n";
    return 0;
}

