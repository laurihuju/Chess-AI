#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <mutex>
#include <atomic>
#include <chrono>
#include "gameState/gameState.h"
#include "move.h"
#include "transpositionTable.h"

class ChessAI {
public:
    /// <summary>
    /// Finds the best next move for the given game state using Minimax algorithm with iterative deepening.
    /// </summary>
    /// <param name="state">The game state to search move for</param>
    /// <param name="maxDepth">The maximum Minimax evaluation depth</param>
    /// <param name="timeLimit">The time limit in milliseconds (default: 4000ms)</param>
    /// <returns>The best move, or Move(0, 0, 0, 0) if no moves found</returns>
    static Move findBestMove(const GameState& state, int maxDepth, int timeLimit = 4000);

private:
    /// <summary>
    /// The currently best value found by the runMinimax function.
    /// The value is reset when starting a new best move search.
    /// </summary>
    static std::atomic<int> bestValue;

    /// <summary>
    /// The index of the game state with the currently best value found by the runMinimax function.
    /// The value is reset when starting a new best move search.
    /// </summary>
    static std::atomic<int> bestValueGameStateIndex;
    
    /// <summary>
    /// Flag indicating whether the time limit has been exceeded.
    /// </summary>
    static std::atomic<bool> timeExceeded;

    /// <summary>
    /// The best move found so far in iterative deepening.
    /// </summary>
    static Move currentBestMove;

    /// <summary>
    /// The transposition table.
    /// </summary>
    static TranspositionTable<30000000> transpositionTable;

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
    /// Recursive implementation of the Minimax algorithm with Alpha-Beta pruning.
    /// </summary>
    /// <param name="state">Current game state</param>
    /// <param name="depth">Current depth in the search tree</param>
    /// <param name="isMaximizingPlayer">Whether current player is maximizing</param>
    /// <param name="playerIsWhite">Whether the evaluating player is white</param>
    /// <param name="alpha">Alpha value for pruning</param>
    /// <param name="beta">Beta value for pruning</param>
    /// <returns>Evaluation score for the current state</returns>
    static int minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite, int alpha = std::numeric_limits<int>::min(), int beta = std::numeric_limits<int>::max());

    /// <summary>
    /// Orders game states by their initial evaluation.
    /// </summary>
    /// <param name="states">Vector of game states to order</param>
    /// <param name="transpositionTableMove">The best move stored in the transposition table, give Move(0, 0, 0, 0) if not available</param>
    /// <param name="isWhite">If evaluation should be done from perspective of white</param>
    static void orderMoves(std::vector<GameState>& states, const Move& transpositionTableMove, bool isWhite);

    /// <summary>
    /// Quiescence search to evaluate tactical positions more accurately.
    /// Only considers capturing moves to reach a "quiet" position.
    /// </summary>
    /// <param name="state">Current game state</param>
    /// <param name="playerIsWhite">Whether the evaluating player is white</param>
    /// <param name="alpha">Alpha value for pruning</param>
    /// <param name="beta">Beta value for pruning</param>
    /// <param name="depth">Current quiescence search depth</param>
    /// <returns>Evaluation score for the quiet position</returns>
    static int quiescenceSearch(const GameState& state, bool playerIsWhite, int alpha, int beta, int depth = 4);

};

#endif