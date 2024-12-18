#include <vector>
#include "Board.hpp"

namespace chess_board_namespace
{
    template <typename T, typename A>
    using container_type = std::vector<T, A>;
    
    typedef std::string value_type;     // std::string because it is not bound to 1 byte of data --> no overflow

    const int CHESS_BOARD_SIZE = 8;
    const value_type EMPTY_SQUARE = "□";

    class ChessBoard{

        public:
            friend std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

        private:
            Board<container_type, value_type> board{CHESS_BOARD_SIZE, CHESS_BOARD_SIZE, EMPTY_SQUARE};

    };

    // Function and Method Definition

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb)
    {
        return os << cb.board;
    }
}