#ifndef CHESS_AI_H
#define CHESS_AI_H

#include "gameState/gameState.h"
#include "move.h"

class ChessAI {
public:
    // Find the best move for the current player using minimax
    static Move findBestMove(const GameState& state, bool isWhite, int depth);

private:
    // Minimax algorithm implementation
    static int minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite);
};

#endif