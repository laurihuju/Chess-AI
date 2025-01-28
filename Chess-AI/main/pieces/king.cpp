#include <vector>
#include "king.h"
#include "../piece.h"
#include "../move.h"
#include "../gameState.h"

King::King(bool isWhite) : Piece(isWhite) {}

void King::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Check all squares around the king
	for (int i = (x - 1 >= 0 ? x - 1 : x); i <= x + 1 && i < 8; i++) {
		for (int j = (y - 1 >= 0 ? y - 1 : y); i <= y + 1 && j < 8; j++) {
			if (i == x && j == y)
				continue;

			Piece* squarePiece = gameState.getPieceAt(x, y);
			if (squarePiece != 0 && squarePiece->isWhite() == isWhite())
				continue;

			moves.push_back(Move(x, y, i, j));
		}
	}

}
