#include <vector>
#include "Board.hpp"
#include "chess_notation.hpp"

namespace cbn
{
    #include "chess_board_constants.hpp"

    template <typename T, typename A>
    using container_type = std::vector<T, A>;
        
    template <typename T>
    using allocator_type = std::allocator<T>;
        
    using value_type = std::string;     // std::string because it is not bound to 1 byte of data --> no overflow

    class ChessBoard{

        public:
            friend std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

            void restore();

            void move(const ChessNotation& xy)
            {
                value_type& square_ref = board[xy.x.integer][xy.x.character];
                board[xy.y.integer][xy.y.character] = square_ref;
                square_ref = EMPTY_SQUARE;
            }

        private:
            bn::Board<container_type, value_type, allocator_type> board{DEFAULT_CHESS_BOARD};

    };

    // Function and Method Definition

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb)
    {
        return os << cb.board;
    }

    void ChessBoard::restore()
    {
        board = DEFAULT_CHESS_BOARD;
    }
}
