#include "chessAI.h"  
#include "piece.h" 
#include <limits>  
#include <thread>
#include <vector>

std::mutex ChessAI::mutex;
int ChessAI::bestValue;
int ChessAI::bestValueGameStateIndex;

Move ChessAI::findBestMove(const GameState& state, bool isWhite, int depth) {
	std::vector<GameState> possibleStates;
	state.possibleNewGameStates(possibleStates, isWhite);
	if (possibleStates.empty()) {
		return Move(0, 0, 0, 0); // Return empty move as there are no moves available
	}

	// Reset the best value
	bestValue = std::numeric_limits<int>::min();
	bestValueGameStateIndex = -1;

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

	// Return empty move as no moves were found that would lead to the best game state
	return Move(0, 0, 0, 0);
}

void ChessAI::runMinimax(const GameState& state, int stateIndex, int depth, bool isWhite) {
	int value = minimax(state, depth, false, isWhite);
	if (value <= bestValue) {
		return;
	}

	mutex.lock();

	bestValue = value;
	bestValueGameStateIndex = stateIndex;

	mutex.unlock();
}

int ChessAI::minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite) {
	// If we've reached the maximum depth or game is over
	if (depth == 0) {
		return state.evaluate(playerIsWhite);
	}

	std::vector<GameState> possibleStates;
	state.possibleNewGameStates(possibleStates, isMaximizingPlayer ? playerIsWhite : !playerIsWhite);

	// If no moves are available, this might be checkmate or stalemate
	if (possibleStates.empty()) {
		if (state.isCheck(isMaximizingPlayer ? playerIsWhite : !playerIsWhite)) {
			return isMaximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
		}
		return 0; // Stalemate
	}

	if (isMaximizingPlayer) {
		int maxEval = std::numeric_limits<int>::min();
		for (const auto& newState : possibleStates) {
			int eval = minimax(newState, depth - 1, false, playerIsWhite);
			maxEval = std::max(maxEval, eval);
		}
		return maxEval;
	}
	else {
		int minEval = std::numeric_limits<int>::max();
		for (const auto& newState : possibleStates) {
			int eval = minimax(newState, depth - 1, true, playerIsWhite);
			minEval = std::min(minEval, eval);
		}
		return minEval;
	}
}
