#include "chessAI.h"  
#include "piece.h" 
#include <limits>  

Move ChessAI::findBestMove(const GameState& state, bool isWhite, int depth) {  
   std::vector<GameState> possibleStates = state.possibleNewGameStates(isWhite);  
   if (possibleStates.empty()) {  
       return Move(0, 0, 0, 0); // No moves available  
   }  

   int bestValue = std::numeric_limits<int>::min();  
   Move bestMove(0, 0, 0, 0);  

   // Find the move that leads to the best evaluation  
   for (const auto& newState : possibleStates) {  
       int value = minimax(newState, depth - 1, false, isWhite);
       if (value > bestValue) {
           bestValue = value;
           // Find which move led to this state by comparing board states
           for (int y = 0; y < 8; y++) {
               for (int x = 0; x < 8; x++) {
                   Piece* piece = state.getPieceAt(x, y);
                   if (piece && piece->isWhite() == isWhite) {
                       std::vector<Move> moves;
                       piece->possibleMoves(moves, x, y, state);
                       for (const Move& move : moves) {
                           GameState testState(state);
                           testState.applyMove(move);
                           if (testState == newState) {
                               bestMove = move;
                               break;
                           }
                       }
                   }
               }
           }
       }
   }

   return bestMove;
}

int ChessAI::minimax(const GameState& state, int depth, bool isMaximizingPlayer, bool playerIsWhite) {
   // If we've reached the maximum depth or game is over
   if (depth == 0) {
       return state.evaluate(playerIsWhite);
   }

   std::vector<GameState> possibleStates = state.possibleNewGameStates(isMaximizingPlayer ? playerIsWhite : !playerIsWhite);

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
   } else {
       int minEval = std::numeric_limits<int>::max();
       for (const auto& newState : possibleStates) {
           int eval = minimax(newState, depth - 1, true, playerIsWhite);
           minEval = std::min(minEval, eval);
       }
       return minEval;
   }
}