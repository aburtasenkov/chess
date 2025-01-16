#pragma once

#include "chess_bot_constants.hpp"
#include "chess_board.hpp"
#include <limits>

namespace cbot
{
    using namespace cbot_constants;

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

    double multiplier_table(const cbn::Piece_type& type, const cbn::ChessCoordinate& location)
    // return multiplier factor for the piece depending on the location
    {
        const auto& table = multiplier_map.at(type);
        return table[location.integer][location.character];
    }

    double board_score(const cbn::ChessBoard& board, const cbn::Piece_color& color)
    // return the board score for color
    // Influenced by piece and its location
    {
        double score = 0;

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

                score += piece_score.at(current_piece.type) * multiplier_table(current_piece.type, current);
            }
        }
        return score;
    }

    class Engine{
    public:
        Engine()  {   }

        double minimax(cbn::ChessBoard board, const int depth = 2)
        {
            auto compare_operator = [](double x, double y){ return x > y; };
            
            if (depth == 0 || board.is_game_over(board.colors_turn()))
                return cbot::board_score(board, board.colors_turn());

            lmn::Legalmoves legal(board);

            double value, best_score;

            // Bot is moving black so search for the best moves
            if (board.colors_turn() == cbn::Piece_color::Black)
            {
                double best_score = std::numeric_limits<double>::lowest();  // lowest best score possible for double as staring point
            }
            else
            {
                // Human is moving white so hope for the worst moves
                double best_score = std::numeric_limits<double>::max();  // highest best score possible for double as staring point
                auto compare_operator = [](double x, double y){ return x < y; };    
            }

            // iterate all legal moves
            for (int rank_index = 0; rank_index < cbn::CHESS_BOARD_SIZE; ++rank_index)
            {
                for (int piece_index = 0; piece_index < cbn::CHESS_BOARD_SIZE; ++piece_index)
                {
                    cbn::ChessCoordinate current{piece_index, rank_index};
                    const cbn::Piece& current_piece = board[current];

                    // if color != moving_color --> continue iterating
                    if (current_piece.color != board.colors_turn())
                        continue;

                    const auto& move_list = legal.get_legal_moves(current);
                    for (const auto& destination : move_list)
                    {
                        cbn::ChessNotation notation{current, destination};
                        cbn::TemporalMove _{board, notation};

                        value = minimax(board, depth - 1);

                        if (compare_operator(value, best_score))
                            best_score = value;
                    }
                }
            }

            return best_score;
        }

        cbn::ChessNotation best_notation(cbn::ChessBoard board, const int depth = 2)
        {
            // Pre-Condition: depth exists and game is not over
            if (depth == 0 || board.is_game_over(board.colors_turn()))
                return cbn::ChessNotation{};

            lmn::Legalmoves legal(board);

            cbn::ChessNotation best_notation;
            double best_score = std::numeric_limits<double>::lowest();  // lowest best score possible for double as staring point

            // iterate all legal moves
            for (int rank_index = 0; rank_index < cbn::CHESS_BOARD_SIZE; ++rank_index)
            {
                for (int piece_index = 0; piece_index < cbn::CHESS_BOARD_SIZE; ++piece_index)
                {
                    cbn::ChessCoordinate current{piece_index, rank_index};
                    const cbn::Piece& current_piece = board[current];

                    // if color != moving_color --> continue iterating
                    if (current_piece.color != board.colors_turn())
                        continue;

                    const auto& move_list = legal.get_legal_moves(current);
                    for (const auto& destination : move_list)
                    {
                        cbn::ChessNotation notation{current, destination};
                        cbn::TemporalMove _{board, notation};

                        double value = minimax(board, depth - 1);

                        if (value > best_score)
                        {
                            best_notation = notation;
                            best_score = value;
                        }
                    }
                }
            }

            return best_notation;
        }
    };
}
