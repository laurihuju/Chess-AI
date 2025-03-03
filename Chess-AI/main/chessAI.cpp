#include "chessAI.h"  
#include "piece.h" 
#include <limits>  
#include <thread>
#include <vector>
#include <algorithm>

std::atomic<int> ChessAI::bestValue;
std::atomic<int> ChessAI::bestValueGameStateIndex;

TranspositionTable<60000000> ChessAI::transpositionTable;

Move ChessAI::findBestMove(const GameState& state, bool isWhite, int depth) {
    std::vector<GameState> possibleStates;
    state.possibleNewGameStates(possibleStates, isWhite);
    if (possibleStates.empty()) {
        return Move(0, 0, 0, 0); // Return empty move as there are no moves available
    }

    // Order moves before evaluation
    orderMoves(possibleStates, isWhite);

    // Reset the best value
    bestValue = std::numeric_limits<int>::min();
    bestValueGameStateIndex = 0; // Initialize to 0 instead of -1 to ensure we always have a valid move

    //for (int i = 0; i < possibleStates.size(); i++) {
    //    runMinimax(possibleStates[i], i, depth - 1, isWhite);
    //}

    // Run Minimax evaluation for every currently possible new GameState in different threads
    std::vector<std::thread*> threads;
    for (int i = 0; i < possibleStates.size(); i++) {
        std::thread* thread = new std::thread(runMinimax, possibleStates[i], i, depth - 1, isWhite);
        threads.push_back(thread);
    }

    // Wait for all threads to finish before continuing
    for (std::thread* thread : threads) {
        thread->join();
        delete thread;
    }

    // Find which move led to this state by comparing board states
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece* piece = state.getPieceAt(x, y);
            if (!piece || piece->isWhite() != isWhite) {
                continue;
            }

            std::vector<Move> moves;
            piece->possibleMoves(moves, x, y, state);
            for (const Move& move : moves) {
                GameState testState(state);
                testState.applyMove(move);

                if (testState == possibleStates[bestValueGameStateIndex]) {
                    return move;
                }
            }
        }
    }

    // If we get here, pick any legal move from the first piece we find
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece* piece = state.getPieceAt(x, y);
            if (!piece || piece->isWhite() != isWhite) {
                continue;
            }

            std::vector<Move> moves;
            piece->possibleMoves(moves, x, y, state);
            for (const Move& move : moves) {
                GameState testState(state);
                testState.applyMove(move);
                if (!testState.isCheck(isWhite)) {
                    return move;
                }
            }
        }
    }

    return Move(0, 0, 0, 0);
}

void ChessAI::orderMoves(std::vector<GameState>& states, bool isWhite) {
    // Create pairs of (evaluation, index) for stable sorting
    std::vector<std::pair<int, int>> evaluations;
    for (int i = 0; i < states.size(); i++) {
        evaluations.push_back({states[i].evaluate(isWhite), i});
    }

    // Sort evaluations in descending order
    std::stable_sort(evaluations.begin(), evaluations.end(), 
        [](const auto& a, const auto& b) { return a.first > b.first; });

    // Reorder states based on sorted evaluations
    std::vector<GameState> orderedStates;
    orderedStates.reserve(states.size());
    for (const auto& eval : evaluations) {
        orderedStates.push_back(std::move(states[eval.second]));
    }
    states = std::move(orderedStates);
}

void ChessAI::runMinimax(const GameState& state, int stateIndex, int depth, bool isWhite) {
    // Calculate the evaluation value of the game tree branch this function evaluates
    int value = minimax(state, depth, false, isWhite);

    // If the evaluation value is better than the previous one, update the best value
    if (value > bestValue || bestValueGameStateIndex == -1) {
        bestValue = value;
        bestValueGameStateIndex = stateIndex;
    }
}

int ChessAI::minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite, int alpha, int beta) {
    // The alpha and beta before the changes made to them in this function
    int alphaOrig = alpha;
    int betaOrig = beta;
    
    // Look up the transposition table
    int transpositionTableEvaluationValue;
    TranspositionTableItemType transpositionTableItemType;
    if (transpositionTable.lookup(state, depth, transpositionTableEvaluationValue, transpositionTableItemType)) {
        // If the stored value is exact minimax value, return it
        if (transpositionTableItemType == TranspositionTableItemType::Exact) {
            return transpositionTableEvaluationValue;
        }
        // If the stored value is lower bound (the exact value is not known due to pruning but the value is at least the stored value), adjust alpha
        else if (transpositionTableItemType == TranspositionTableItemType::LowerBound) {
            alpha = std::max(alpha, transpositionTableEvaluationValue);
        }
        // If the stored value is lower bound (the exact value is not known due to pruning but the value is at most the stored value), adjust beta
        else if (transpositionTableItemType == TranspositionTableItemType::UpperBound) {
            beta = std::min(beta, transpositionTableEvaluationValue);
        }

        // Alpha-beta pruning
        if (alpha >= beta) {
            return transpositionTableEvaluationValue;
        }
    }
    
    // If we've reached the maximum depth or game is over
    if (depth == 0) {
        return state.evaluate(playerIsWhite);
    }

    // Fetch the possible new game states that can be made from the current evaluation game state with one move
    std::vector<GameState> possibleStates;
    state.possibleNewGameStates(possibleStates, isMaximizingPlayer ? playerIsWhite : !playerIsWhite);

    // If no moves are available, this is checkmate or stalemate
    if (possibleStates.empty()) {
        if (state.isCheck(isMaximizingPlayer ? playerIsWhite : !playerIsWhite)) {
            return isMaximizingPlayer ? -10000 + depth : 10000 - depth;
        }
        return 0; // Stalemate
    }

    // Order moves before evaluation
    orderMoves(possibleStates, isMaximizingPlayer ? playerIsWhite : !playerIsWhite);

    // The best evaluation value found for the game state
    int bestEval;

    // Handle the maximizer's turn
    if (isMaximizingPlayer) {
        bestEval = std::numeric_limits<int>::min();
        for (const auto& newState : possibleStates) {
            int eval = minimax(newState, depth - 1, false, playerIsWhite, alpha, beta);
            bestEval = std::max(bestEval, eval);
            alpha = std::max(alpha, eval);
            
            // Alpha-beta pruning
            if (beta <= alpha) {
                break;
            }
        }
    // Handle the minimizer's turn
    } else {
        bestEval = std::numeric_limits<int>::max();
        for (const auto& newState : possibleStates) {
            int eval = minimax(newState, depth - 1, true, playerIsWhite, alpha, beta);
            bestEval = std::min(bestEval, eval);
            beta = std::min(beta, eval);

            // Alpha-beta pruning
            if (beta <= alpha) {
                break;
            }
        }
    }

    // Calculate the transposition table item type of this node
    TranspositionTableItemType transpositionItemType;
    if (bestEval <= alphaOrig) {
        transpositionItemType = TranspositionTableItemType::UpperBound;
    }
    else if (bestEval >= betaOrig) {
        transpositionItemType = TranspositionTableItemType::LowerBound;
    }
    else {
        transpositionItemType = TranspositionTableItemType::Exact;
    }

    // Store the result of the evaluation of this game state to the transposition table
    transpositionTable.store(state, bestEval, depth, transpositionItemType);

    // Return the evaluation value of this game state
    return bestEval;
}
