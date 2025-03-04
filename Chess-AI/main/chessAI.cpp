#include "chessAI.h"  
#include "piece.h" 
#include <limits>  
#include <thread>
#include <vector>
#include <algorithm>

std::atomic<int> ChessAI::bestValue;
std::atomic<int> ChessAI::bestValueGameStateIndex;

TranspositionTable<30000000> ChessAI::transpositionTable;

Move ChessAI::findBestMove(const GameState& state, bool isWhite, int depth) {
    std::vector<GameState> possibleStates;
    state.possibleNewGameStates(possibleStates);
    if (possibleStates.empty()) {
        return Move(0, 0, 0, 0); // Return empty move as there are no moves available
    }

    // Order moves before evaluation
    orderMoves(possibleStates, Move(0, 0, 0, 0), isWhite);

    // Reset the best value
    bestValue = std::numeric_limits<int>::min();
    bestValueGameStateIndex = 0; // Initialize to 0 instead of -1 to ensure we always have a valid move

	//// Code to be used instead of the multithreading code when debugging needs
    //// fully deterministic single-threaded algorithm
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
            return isMaximizingPlayer ? -10000 + depth : 10000 - depth;
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

void ChessAI::getCapturingMoves(const GameState& state, bool isWhite, std::vector<GameState>& capturingStates) {
	state.possibleNewGameStates(capturingStates);

	// Filter for capturing moves only
	for (int i = 0; i < capturingStates.size(); i++) {
		int materialDiff = capturingStates[i].evaluationValue(isWhite) - state.evaluationValue(isWhite);

		// Only include if material difference indicates a capture occurred
		if (std::abs(materialDiff) < 100) { // 100 is minimum piece value (pawn)
			capturingStates.erase(capturingStates.begin() + i);
			i--;
		}
	}
}

int ChessAI::quiescenceSearch(const GameState& state, bool playerIsWhite, int alpha, int beta, int depth) {
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
    getCapturingMoves(state, playerIsWhite, capturingStates);

    // Order moves for better pruning
    orderMoves(capturingStates, Move(0, 0, 0, 0), playerIsWhite);

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
