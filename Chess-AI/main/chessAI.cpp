#include "chessAI.h"  
#include "piece.h" 
#include <limits>  
#include <thread>
#include <vector>
#include <algorithm>

std::mutex ChessAI::mutex;
int ChessAI::bestValue;
int ChessAI::bestValueGameStateIndex;

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

void ChessAI::runMinimax(const GameState& state, int stateIndex, int depth, bool isWhite) {
    int value = minimax(state, depth, false, isWhite);
    
    if (value > bestValue || bestValueGameStateIndex == -1) {
        mutex.lock();

        bestValue = value;
        bestValueGameStateIndex = stateIndex;

        mutex.unlock();
    }

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

int ChessAI::minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite, int alpha, int beta) {
    if (depth == 0) {
        return quiescenceSearch(state, playerIsWhite, alpha, beta);
    }

    std::vector<GameState> possibleStates;
    state.possibleNewGameStates(possibleStates, isMaximizingPlayer ? playerIsWhite : !playerIsWhite);

    if (possibleStates.empty()) {
        if (state.isCheck(isMaximizingPlayer ? playerIsWhite : !playerIsWhite)) {
            return isMaximizingPlayer ? -10000 + depth : 10000 - depth;
        }
        return 0; // Stalemate
    }

    // Order moves before evaluation
    orderMoves(possibleStates, isMaximizingPlayer ? playerIsWhite : !playerIsWhite);

    if (isMaximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& newState : possibleStates) {
            int eval = minimax(newState, depth - 1, false, playerIsWhite, alpha, beta);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& newState : possibleStates) {
            int eval = minimax(newState, depth - 1, true, playerIsWhite, alpha, beta);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

void ChessAI::getCapturingMoves(const GameState& state, bool isWhite, std::vector<GameState>& capturingStates) {
    std::vector<GameState> allStates;
    state.possibleNewGameStates(allStates, isWhite);
    
    // Filter for capturing moves only
    for (const auto& newState : allStates) {
        int materialDiff = newState.evaluate(isWhite) - state.evaluate(isWhite);
        // Only include if material difference indicates a capture occurred
        if (std::abs(materialDiff) >= 100) { // 100 is minimum piece value (pawn)
            capturingStates.push_back(newState);
        }
    }
}

int ChessAI::quiescenceSearch(const GameState& state, bool playerIsWhite, int alpha, int beta, int depth) {
    // Base evaluation
    int standPat = state.evaluate(playerIsWhite);
    
    // Return if maximum depth or checkmate
    if (depth == 0) {
        return standPat;
    }

    // Beta cutoff
    if (standPat >= beta) {
        return beta;
    }

    // Update alpha if standing pat is better
    if (standPat > alpha) {
        alpha = standPat;
    }

    // Get capturing moves only
    std::vector<GameState> capturingStates;
    getCapturingMoves(state, playerIsWhite, capturingStates);

    // Order moves for better pruning
    orderMoves(capturingStates, playerIsWhite);

    // Search capturing moves
    for (const auto& newState : capturingStates) {
        int score = -quiescenceSearch(newState, !playerIsWhite, -beta, -alpha, depth - 1);
        
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}
