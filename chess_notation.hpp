#include <iostream>
#include <assert.h>

const int CHESSMOVE_INVALID_VALUE = -1;
const char ALPHABET_TO_INT = '`'; // character 1 int smaller than 'a' --> 'a' has index 1
const int INDEX_TO_NUM = 1;

struct ChessMove{
    ChessMove ()
        :character(CHESSMOVE_INVALID_VALUE), integer(CHESSMOVE_INVALID_VALUE) {   }

    ChessMove(int c, int i) 
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

std::ostream& operator<<(std::ostream& os, const ChessMove& move)
{
    return os << (char)(ALPHABET_TO_INT + move.character) << move.integer + INDEX_TO_NUM;
}

std::istream& operator>>(std::istream& is, ChessMove& move)
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