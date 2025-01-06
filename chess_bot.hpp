#include "chess_board.hpp"
#include <limits>

namespace cbot
{
    bool coordinate_in_range(const cbn::ChessCoordinate& A, const cbn::ChessCoordinate& B, const cbn::ChessCoordinate& C)
    // return true if C is somewhere between A and B
    {
        // C is bigger min(A,B) and max(A,B)

        if (!(std::min(A.character, B.character) <= C.character && C.character <= std::max(A.character, B.character)))
            return false;
        
        if (!(std::min(A.integer, B.integer) <= C.integer && C.integer <= std::max(A.integer, B.integer)))
            return false;
        
        return true;
    }

    int multiplier_table(const cbn::ChessCoordinate& location)
    // return multiplier factor for the piece depending on the location
    {
        auto square_list = cbn::coordinates_between_xy(location, cbn::ChessCoordinate{cbn::CHESS_BOARD_SIZE / 2, cbn::CHESS_BOARD_SIZE / 2});
        return 8 - square_list.size();
    }

    int board_score(const cbn::ChessBoard& board, const cbn::Piece_color& color)
    // return the board score for color
    // Influenced by piece and its location
    {
        int score = 0;

        // iterate over each piece of the board
        for (int rank_index = 0; rank_index < cbn::CHESS_BOARD_SIZE; ++rank_index)
        {
            for (int piece_index = 0; piece_index < cbn::CHESS_BOARD_SIZE; ++piece_index)
            {
                cbn::ChessCoordinate current{piece_index, rank_index};
                const cbn::Piece& current_piece = board[current];

                // if not color
                if (current_piece.color != color)
                    continue;

                score += cbn::piece_score.at(current_piece.type) * multiplier_table(current);
            }
        }
        return score;
    }
}

class ChessBot{
public:
    ChessBot()  {   }

    cbn::ChessNotation next_move(cbn::ChessBoard board, const cbn::Piece_color& moving_color, const int iterations)
    {
        // iterate over each piece of the board
        for (int rank_index = 0; rank_index < cbn::CHESS_BOARD_SIZE; ++rank_index)
        {
            for (int piece_index = 0; piece_index < cbn::CHESS_BOARD_SIZE; ++piece_index)
            {
                cbn::ChessCoordinate current{piece_index, rank_index};
                const cbn::Piece& current_piece = board[current];

                // if color != moving_color --> continue iterating
                if (current_piece.color != moving_color)
                    continue;
            }
        }
        // best_move(cbn::ChessBoard, const cbn::ChessCoordinate& move_from, const int iterations):
        //      - get all legal moves for the current piece, for each piece ->
        //          - TemporalMove it
        //          - calculate current score ( influenced by piece it ate and location of the piece )
        //          - save best score and the corresponding move
        // after iterating over each move
        return cbn::ChessNotation{};
    }
    cbn::ChessNotation best_move(cbn::ChessBoard& board, const cbn::ChessCoordinate& move_from)
    // return the current best move for move_from
    // Prototype, TODO
    {
        lmn::Legalmoves legal(board);
        cbn::Piece current_piece = board[move_from];

        cbn::ChessNotation best_notation{};
        int best_score = std::numeric_limits<int>::lowest();  // lowest best score possible for int as staring point

        // get all legal moves for the current piece
        const auto& move_to_list = legal.get_legal_moves(move_from);

        // iterate over them
        for (const auto& move_to : move_to_list)
        {
            // TemporalMove it
            cbn::ChessNotation notation{move_from, move_to};
            cbn::TemporalMove{board, notation};

            // calculate current score
            int score = cbot::board_score(board, current_piece.color);

            if (score > best_score)
            {
                best_notation = notation;
                best_score = score;
            }
        }
        return best_notation;
    }
};