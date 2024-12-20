#include "chess_board.hpp"
using namespace cbn;

ChessBoard board;

int main()
try {
    // while (true)
    // {
    //     std::cout << board;
    //     ChessCoordinate x, y;
    //     std::cin >> x >> y;
    //     board.move({x,y});
    // }
    std::cout << board;
    ChessCoordinate x;
    std::cin >> x;
    Legalmoves lm {board, x};
    std::vector<ChessCoordinate> legal_moves = lm();
    std::cout << legal_moves.size() << "\n";
    std::cout << board;
    return 0;
}
catch (Exception& e)
{
    std::cerr << e.what() << "\n";
}

