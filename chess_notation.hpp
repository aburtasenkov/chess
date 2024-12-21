#include <iostream>
#include <assert.h>
#include "Exception.hpp"

namespace chess_notation
{
    const int CHESSCOORDINATE_INVALID_VALUE = -1;
    const char ALPHABET_TO_INT = '`'; // character 1 int smaller than 'a' --> 'a' has index 1
    const int INDEX_TO_NUM = 1;
    const int RANK_1_INDEX = 0;
    const int RANK_2_INDEX = 1;
    const int RANK_3_INDEX = 2;
    const int RANK_4_INDEX = 3;
    const int RANK_5_INDEX = 4;
    const int RANK_6_INDEX = 5;
    const int RANK_7_INDEX = 6;
    const int RANK_8_INDEX = 7;

    const Exception BadInputError{"Bad Input: Piece Location"};
    const Exception EmptySquareError{"BadInput: Location Does Not Contain Any Pieces"};

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
            if (!(0 < character && character < 9))  // index for column between 1 and 8 incl.
                throw BadInputError;
            if (!(-1 < integer && integer < 8))    // index for row between 0 and 7 incl.
                throw BadInputError;
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

    ChessCoordinate operator+(const ChessCoordinate& location, const ChessCoordinate& relocation)
    {
        return ChessCoordinate{location.character + relocation.character, location.integer + relocation.integer};
    }

    ChessCoordinate operator-(const ChessCoordinate& location, const ChessCoordinate& relocation)
    {
        return ChessCoordinate{location.character - relocation.character, location.integer - relocation.integer};
    }
}