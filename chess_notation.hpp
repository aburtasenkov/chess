#include <iostream>
#include <assert.h>

namespace chess_notation
{
    const int CHESSCOORDINATE_INVALID_VALUE = -1;
    const char ALPHABET_TO_INT = '`'; // character 1 int smaller than 'a' --> 'a' has index 1
    const int INDEX_TO_NUM = 1;

    struct ChessCoordinate{
        ChessCoordinate ()
            :character(CHESSCOORDINATE_INVALID_VALUE), integer(CHESSCOORDINATE_INVALID_VALUE) {   }

        ChessCoordinate(int c, int i) 
        {    
            assert_data();
            character = c;
            integer = i;
        }

        void assert_data()
        {
            assert(0 < character && character < 9); // index for column between 1 and 8 incl.
            assert(-1 < integer && integer < 8);    // index for row between 0 and 7 incl.
        }

        // both are integer values, so I can use them for indexing ChessBoard[integer][character]
        int character;
        int integer;
    };

    std::ostream& operator<<(std::ostream& os, const ChessCoordinate& move)
    {
        return os << (char)(ALPHABET_TO_INT + move.character) << move.integer + INDEX_TO_NUM;
    }

    std::istream& operator>>(std::istream& is, ChessCoordinate& move)
    {
        char ch;
        int i;

        is >> ch >> i;
        assert(is.good());

        move.character = tolower(ch) - ALPHABET_TO_INT;
        move.integer = i - INDEX_TO_NUM;

        move.assert_data();

        return is;
    } 

    bool operator==(const ChessCoordinate& x, const ChessCoordinate& y)
    {
        return ((x.character == y.character) && (x.integer == y.integer));
    }

    bool operator!=(const ChessCoordinate& x, const ChessCoordinate& y)
    {
        return !operator==(x,y);
    }

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

    std::ostream& operator<<(std::ostream& os, const ChessNotation& notation)
    {
        return os << "From: " << notation.x << " To: " << notation.y; 
    }

    std::istream& operator>>(std::istream& is, ChessNotation& notation)
    {
        is >> notation.x >> notation.y;
        notation.x.assert_data();
        notation.y.assert_data();
        return is; 
    } 
}