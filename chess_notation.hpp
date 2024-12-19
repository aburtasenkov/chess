#include "chess_coordinate.hpp"

struct ChessNotation
{
    ChessNotation()
        :x(ChessCoordinate{}), y(ChessCoordinate{}) {   }

    ChessNotation(const ChessCoordinate& f, const ChessCoordinate& t)
        :x(f), y(t) 
    {
        x.assert_data();
        y.assert_data();
        assert(x != y);
    }

    ChessCoordinate x;  // move from x
    ChessCoordinate y;  // to y
};