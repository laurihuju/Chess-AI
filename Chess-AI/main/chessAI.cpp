#include "chessAI.h"  
#include "piece.h" 
#include <limits>  
#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>

std::atomic<int> ChessAI::bestValue;
std::atomic<int> ChessAI::bestValueGameStateIndex;
std::atomic<bool> ChessAI::timeExceeded;
std::chrono::time_point<std::chrono::high_resolution_clock> ChessAI::startTime;
int ChessAI::searchTimeLimit;
Move ChessAI::currentBestMove = Move(0, 0, 0, 0);

TranspositionTable<30000000> ChessAI::transpositionTable;

Move ChessAI::findBestMove(const GameState& state, int maxDepth, int timeLimit) {
    std::vector<GameState> possibleStates;
    state.possibleNewGameStates(possibleStates);
    if (possibleStates.empty()) {
        return Move(0, 0, 0, 0); // Return empty move as there are no moves available
    }

    // Initialize time tracking
    startTime = std::chrono::high_resolution_clock::now();
    searchTimeLimit = timeLimit;
    timeExceeded = false;
    
    // Initialize the best move to the first possible move as a fallback
    currentBestMove = possibleStates[0].lastMove();
    
    // Iterative deepening
    // Start with depth 1 to quickly find mates in 1 move
    // Then continue with depths 2, 4, 6, ... up to maxDepth
    for (int depth = 1; depth <= maxDepth; depth = (depth == 1) ? 2 : depth + 2) {
        // Order moves before evaluation
        orderMoves(possibleStates, currentBestMove, state.isWhiteSideToMove());

        // Reset the best value for this depth iteration
        bestValue = std::numeric_limits<int>::min();
        bestValueGameStateIndex = 0;
        
        // Run Minimax evaluation for every currently possible new GameState in different threads
        std::vector<std::thread*> threads;
        for (int i = 0; i < possibleStates.size(); i++) {
            std::thread* thread = new std::thread(runMinimax, possibleStates[i], i, depth - 1, state.isWhiteSideToMove());
            threads.push_back(thread);
        }

        // Wait for all threads to finish before continuing
        for (std::thread* thread : threads) {
            thread->join();
            delete thread;
        }

        // Store the best move for this depth iteration if not out of time
        if (!timeExceeded) {
            currentBestMove = possibleStates[bestValueGameStateIndex].lastMove();
            
            // Debug output
            std::cout << "Depth " << depth << " completed. Best move: (" 
                      << (int)currentBestMove.x1() << "," << (int)currentBestMove.y1() << ") -> (" 
                      << (int)currentBestMove.x2() << "," << (int)currentBestMove.y2() << ")" << std::endl;
            
            // If we found a checkmate, no need to search deeper
            if (bestValue > 900000 || bestValue < -900000) {
                break;
            }
        } else {
            std::cout << "Depth " << depth << " exceeded time limit. Using best move from depth " 
                      << ((depth == 1) ? 1 : depth - 2) << std::endl;
            break;
        }
    }

    // Return the best move found
    return currentBestMove;
}

bool ChessAI::isTimeExceeded() {
    if (timeExceeded) return true;
    
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    
    if (elapsed >= searchTimeLimit) {
        timeExceeded = true;
        return true;
    }
    
    return false;
}

void ChessAI::orderMoves(std::vector<GameState>& states, const Move& transpositionTableMove, bool isWhite) {
	std::sort(states.begin(), states.end(), [isWhite, transpositionTableMove](const GameState& a, const GameState& b) {
		if (a.lastMove() == transpositionTableMove) {
			return true;
		}
		if (b.lastMove() == transpositionTableMove) {
			return false;
		}

		return a.evaluationValue(isWhite) > b.evaluationValue(isWhite);
    });
}

void ChessAI::runMinimax(const GameState& state, int stateIndex, int depth, bool isWhite) {
    // Stop evaluation if time is exceeded
    if (isTimeExceeded()) {
        return;
    }
    
    // Calculate the evaluation value of the game tree branch this function evaluates
    int value = minimax(state, depth, false, isWhite);

    // If the evaluation value is better than the previous one, update the best value
    if (value > bestValue || bestValueGameStateIndex == -1) {
        bestValue = value;
        bestValueGameStateIndex = stateIndex;
    }
}

int ChessAI::minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite, int alpha, int beta) {
    // Check if time is exceeded
    if (isTimeExceeded()) {
        // Return a neutral value that won't affect the search
        return 0;
    }
    
    // The alpha and beta before the changes made to them in this function
    int alphaOrig = alpha;
    int betaOrig = beta;
    
    // Look up the transposition table
    int transpositionTableEvaluationValue;
	Move transpositionTableMove = Move(0, 0, 0, 0);
    TranspositionTableItemType transpositionTableItemType;
    if (transpositionTable.lookup(state, depth, transpositionTableEvaluationValue, transpositionTableMove, transpositionTableItemType)) {
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
        return quiescenceSearch(state, playerIsWhite, alpha, beta);
    }

    // Fetch the possible new game states that can be made from the current evaluation game state with one move
    std::vector<GameState> possibleStates;
    state.possibleNewGameStates(possibleStates);

    // If no moves are available, this is checkmate or stalemate
    if (possibleStates.empty()) {
        if (state.isCheck(isMaximizingPlayer ? playerIsWhite : !playerIsWhite)) {
            return isMaximizingPlayer ? -1000000 - (depth * 10000) : 1000000 + (depth * 10000);
        }
        return 0; // Stalemate
    }

    // Order moves before evaluation
    orderMoves(possibleStates, transpositionTableMove, isMaximizingPlayer ? playerIsWhite : !playerIsWhite);

    // The best evaluation value and move found for the game state
    int bestEval;
	Move bestMove = Move(0, 0, 0, 0);

    // Handle the maximizer's turn
    if (isMaximizingPlayer) {
        bestEval = std::numeric_limits<int>::min();
        for (const auto& newState : possibleStates) {
            // Check time limit before recursing
            if (isTimeExceeded()) {
                return 0;
            }
            
            int eval = minimax(newState, depth - 1, false, playerIsWhite, alpha, beta);
            alpha = std::max(alpha, eval);
			if (eval > bestEval) {
				bestEval = eval;
				bestMove = newState.lastMove();
			}
            
            // Alpha-beta pruning
            if (beta <= alpha) {
                break;
            }
        }
    // Handle the minimizer's turn
    } else {
        bestEval = std::numeric_limits<int>::max();
        for (const auto& newState : possibleStates) {
            // Check time limit before recursing
            if (isTimeExceeded()) {
                return 0;
            }
            
            int eval = minimax(newState, depth - 1, true, playerIsWhite, alpha, beta);
            beta = std::min(beta, eval);
            if (eval < bestEval) {
                bestEval = eval;
                bestMove = newState.lastMove();
            }

            // Alpha-beta pruning
            if (beta <= alpha) {
                break;
            }
        }
    }

    // If time is exceeded, don't store in the transposition table
    if (isTimeExceeded()) {
        return bestEval;
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
    transpositionTable.store(state, bestEval, depth, bestMove, transpositionItemType);

    // Return the evaluation value of this game state
    return bestEval;
}

int ChessAI::quiescenceSearch(const GameState& state, bool playerIsWhite, int alpha, int beta, int depth) {
    // Check if time is exceeded
    if (isTimeExceeded()) {
        return 0;
    }
    
    // Base evaluation
    int standPat = state.evaluationValue(playerIsWhite);
    
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
    state.possibleNewGameStates(capturingStates, true);

    // Order moves for better pruning
    orderMoves(capturingStates, Move(0, 0, 0, 0), playerIsWhite);

    // Search capturing moves
    for (const auto& newState : capturingStates) {
        // Check time limit
        if (isTimeExceeded()) {
            return alpha;
        }
        
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
