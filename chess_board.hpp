#include <vector>
#include "Board.hpp"

namespace cbn
{
    #include "chess_board_constants.hpp"

    class ChessBoard{

        public:
            friend std::ostream& operator<<(std::ostream& os, const ChessBoard& cb);

            void restore();

        private:
            Board<container_type, value_type, allocator_type> board{DEFAULT_CHESS_BOARD};

    };

    // Function and Method Definition

    std::ostream& operator<<(std::ostream& os, const ChessBoard& cb)
    {
        return os << cb.board;
    }
}
