#include <vector>
#include "Board.hpp"

Board<std::vector, char> chess{ 8, 8, 'X' };

int main()
{
    chess.output(std::cout);
    return 0;
}

