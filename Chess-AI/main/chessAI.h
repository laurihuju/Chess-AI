#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <mutex>
#include "gameState/gameState.h"
#include "move.h"

class ChessAI {
public:
    /// <summary>
    /// Finds the best move for the given player using Minimax algorithm.
    /// </summary>
    /// <param name="state">The current game state</param>
    /// <param name="isWhite">If the best move should be searched for white</param>
    /// <param name="depth">The Minimax evaluation depth</param>
    /// <returns>The best move, or move from (0; 0) to (0; 0) if no moves found</returns>
    static Move findBestMove(const GameState& state, bool isWhite, int depth);

private:
    /// <summary>
    /// The mutex used for writing the value of bestValue and bestValueGameStateIndex.
    /// </summary>
    static std::mutex mutex;

    /// <summary>
    /// The currently best value found by the runMinimax function.
    /// The value is reseted when starting a new best move search.
    /// </summary>
    static int bestValue;

    /// <summary>
    /// The index of the game state with the currently best value found by the runMinimax function.
    /// The value is reseted when starting a new best move search.
    /// </summary>
    static int bestValueGameStateIndex;

    /// <summary>
    /// Thread safe function that evaluates the given GameState with the minimax function.
    /// If the minimax result is greater than the value of bestValue, sets the result as the value
    /// of bestValue and the stateIndex as the value of bestValueGameStateIndex.
    /// </summary>
    /// <param name="state">The game state this function should evaluate with Minimax</param>
    /// <param name="stateIndex">The index of the given state</param>
    /// <param name="depth">The evaluation depth</param>
    /// <param name="isWhite">If evaluation should be done from the perspective of white</param>
    static void runMinimax(const GameState& state, int stateIndex, int depth, bool isWhite);
    
    /// <summary>
    /// Recursive implementation of the Minimax algorithm.
    /// </summary>
    /// <param name="state"></param>
    /// <param name="depth"></param>
    /// <param name="isMaximizingPlayer"></param>
    /// <param name="playerIsWhite"></param>
    /// <returns></returns>
    static int minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite);
};

#endif