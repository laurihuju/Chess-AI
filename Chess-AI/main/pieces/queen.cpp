#include <vector>
#include "queen.h"
#include "../move.h"
#include "../gameState.h"

Queen::Queen(bool isWhite) : Bishop(isWhite), Rook(isWhite), Piece(isWhite) {}

std::vector<Move> Queen::possibleMoves(int x, int y, const GameState& gameState) const {
    std::vector<Move> moves;
    
    // Get diagonal moves from Bishop
    std::vector<Move> bishopMoves = Bishop::possibleMoves(x, y, gameState);
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    
    // Get straight moves from Rook
    std::vector<Move> rookMoves = Rook::possibleMoves(x, y, gameState);
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    
    return moves;
}
