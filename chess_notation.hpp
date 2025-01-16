#pragma once

#include <iostream>
#include <assert.h>

#include "Exception.hpp"

namespace chess_notation
{
    const int CHESSCOORDINATE_INVALID_VALUE = -1;

    const int MAX_INDEX = 7;
    const int MIN_INDEX = 0;

    const char ALPHABET_TO_INT = 'a'; // character 1 int smaller than 'a' --> 'a' has index 1
    const int INDEX_TO_NUM = 1;
    const int RANK_1_INDEX = 0;
    const int RANK_2_INDEX = 1;
    const int RANK_3_INDEX = 2;
    const int RANK_4_INDEX = 3;
    const int RANK_5_INDEX = 4;
    const int RANK_6_INDEX = 5;
    const int RANK_7_INDEX = 6;
    const int RANK_8_INDEX = 7;

    const int WHITE_BACK_RANK = RANK_8_INDEX;
    const int BLACK_BACK_RANK = RANK_1_INDEX;

    const Exception BadInputError{"BadInputError: Piece Location"};
    const Exception EmptySquareError{"EmptySquareError: Location Does Not Contain Any Pieces"};

    struct ChessCoordinate{
        ChessCoordinate ()
            :character(CHESSCOORDINATE_INVALID_VALUE), integer(CHESSCOORDINATE_INVALID_VALUE) {   }

        ChessCoordinate(int c, int i) 
        {    
            character = c;
            integer = i;
        }

        ChessCoordinate(const ChessCoordinate& c)
        {
            character = c.character;
            integer = c.integer;
        }

        bool is_valid() const
        {
            if (!(MIN_INDEX <= character && character <= MAX_INDEX))
                return false;
            if (!(MIN_INDEX <= integer && integer <= MAX_INDEX))
                return false;
            return true;
        }

        void assert_data() const
        {
            if (!(MIN_INDEX <= character && character <= MAX_INDEX))
                throw BadInputError;
            if (!(MIN_INDEX <= integer && integer <= MAX_INDEX))
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

    bool operator<(const ChessCoordinate& x, const ChessCoordinate& y)
    {
        if (x.character < y.character)
            return true;
        else if (x.character == y.character)
        {
            if (x.integer < y.integer)
                return true;
        }

        return false;
    }

    bool operator>(const ChessCoordinate& x, const ChessCoordinate& y)
    {
        if (x.character > y.character)
            return true;
        else if (x.character == y.character)
        {
            if (x.integer > y.integer)
                return true;
        }

        return false;
    }

    bool operator<=(const ChessCoordinate& x, const ChessCoordinate& y)
    {
        if (x == y)
            return true;
        else 
            return x < y;
    }

    bool operator>=(const ChessCoordinate& x, const ChessCoordinate& y)
    {
        if (x == y)
            return true;
        else 
            return x > y;
    }

    ChessCoordinate operator+(const ChessCoordinate& location, const ChessCoordinate& relocation)
    {
        return ChessCoordinate{location.character + relocation.character, location.integer + relocation.integer};
    }

    ChessCoordinate operator-(const ChessCoordinate& location, const ChessCoordinate& relocation)
    {
        return ChessCoordinate{location.character - relocation.character, location.integer - relocation.integer};
    }

    ChessCoordinate& operator+=(ChessCoordinate& location, const ChessCoordinate& shift)
    {
        location.character += shift.character;
        location.integer += shift.integer;
        return location;
    }

    ChessCoordinate& operator-=(ChessCoordinate& location, const ChessCoordinate& shift)
    {
        location.character -= shift.character;
        location.integer -= shift.integer;
        return location;
    }

    struct ChessNotation
    {
        ChessNotation()
            :from(ChessCoordinate{}), to(ChessCoordinate{}) {   }

        ChessNotation(const ChessCoordinate& f, const ChessCoordinate& t)
            :from(f), to(t) 
        {
            from.assert_data();
            to.assert_data();
            assert(from != to);
        }

        ChessCoordinate from;  // move from x
        ChessCoordinate to;  // to y
    };

    std::ostream& operator<<(std::ostream& os, const ChessNotation& notation)
    {
        return os << "From: " << notation.from << " To: " << notation.to; 
    }

    std::istream& operator>>(std::istream& is, ChessNotation& notation)
    {
        is >> notation.from >> notation.to;
        notation.from.assert_data();
        notation.to.assert_data();
        return is; 
    } 

    bool operator==(const ChessNotation& x, const ChessNotation& y)
    {
        return (x.from == y.from && x.to == y.to);
    }

    bool operator!=(const ChessNotation& x, const ChessNotation& y)
    {
        return !operator==(x, y);
    }
}