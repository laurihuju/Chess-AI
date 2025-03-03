#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <mutex>
#include "gameState/gameState.h"
#include "move.h"
#include "transpositionTable.h"

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
    /// The currently best value found by the runMinimax function.
    /// The value is reseted when starting a new best move search.
    /// </summary>
    static std::atomic<int> bestValue;

    /// <summary>
    /// The index of the game state with the currently best value found by the runMinimax function.
    /// The value is reseted when starting a new best move search.
    /// </summary>
    static std::atomic<int> bestValueGameStateIndex;

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

    /// <summary>
    /// Gets only the capturing moves from a state.
    /// </summary>
    /// <param name="state">The game state to analyze</param>
    /// <param name="isWhite">Whether to get moves for white</param>
    /// <param name="capturingStates">Vector to store states after capturing moves</param>
    static void getCapturingMoves(const GameState& state, bool isWhite, std::vector<GameState>& capturingStates);
};

#endif