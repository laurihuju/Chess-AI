#include <vector>
#include <cmath>
#include "pawn.h"
#include "../move.h"
#include "../piece.h"
#include "../gameState/gameState.h"

Pawn::Pawn(bool isWhite) : Piece(isWhite) {}

void Pawn::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// There are no possible moves when the pawn is at the top or bottom row
	if (y == 0 || y == 7) {
		return;
	}
	
	// The possible movement direction (white moves up and black down)
	int movementDirection = isWhite() ? -1 : 1;

	// Capturing
	for (int i = (x - 1 > 0 ? x - 1 : 0); i <= x + 1 && i <= 7; i++) {
		// Ignore the square in front of this piece
		if (i == x) {
			continue;
		}

		// Check if there is opponent's piece at the square
		Piece* pieceToCapture = gameState.getPieceAt(i, y + movementDirection);
		if (pieceToCapture == 0 || pieceToCapture->isWhite() == isWhite()) {
			continue;
		}

		// Capturing move without promotion
		if (y + movementDirection != 0 && y + movementDirection != 7) {
			moves.push_back(Move(x, y, i, y + movementDirection));
			continue;
		}

		// Capturing move with promotion
		moves.push_back(Move(x, y, i, y + movementDirection, 'q'));
		moves.push_back(Move(x, y, i, y + movementDirection, 'n'));
		moves.push_back(Move(x, y, i, y + movementDirection, 'b'));
		moves.push_back(Move(x, y, i, y + movementDirection, 'r'));
	}

	// En passant
	if (movementDirection == -1 && y == 3 && std::abs(gameState.upperEnPassantColumn() - x) == 1) {
		moves.push_back(Move(x, y, gameState.upperEnPassantColumn(), 2));
	} else if (movementDirection == 1 && y == 4 && std::abs(gameState.lowerEnPassantColumn() - x) == 1) {
		moves.push_back(Move(x, y, gameState.lowerEnPassantColumn(), 5));
	}

	// Prevent moving forward when there is an obstacle
	if (gameState.getPieceAt(x, y + movementDirection) != 0) {
		return;
	}

	// Normal forward move without promotion
	if ((movementDirection == -1 && y > 1) || (movementDirection == 1 && y < 6)) {
		moves.push_back(Move(x, y, x, y + movementDirection));
	}
	// Normal forward move with promotion
	else {
		moves.push_back(Move(x, y, x, y + movementDirection, 'q'));
		moves.push_back(Move(x, y, x, y + movementDirection, 'n'));
		moves.push_back(Move(x, y, x, y + movementDirection, 'b'));
		moves.push_back(Move(x, y, x, y + movementDirection, 'r'));
	}

	// Double move
	if (((y == 6 && movementDirection == -1) || (y == 1 && movementDirection == 1)) && gameState.getPieceAt(x, y + movementDirection * 2) == 0) {
		moves.push_back(Move(x, y, x, y + movementDirection * 2));
	}
}

bool Pawn::threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const {
	if (std::abs(squareX - ownX) != 1) {
		return false;
	}

	int movementDirection = isWhite() ? -1 : 1;

	// Upper en passant
	if (movementDirection == 1 && ownY == 3 && squareY == 3 && gameState.upperEnPassantColumn() == squareX) {
		return true;
	}

	// Lower en passant
	if (movementDirection == -1 && ownY == 4 && squareY == 4 && gameState.lowerEnPassantColumn() == squareX) {
		return true;
	}

	// Capturing
	return squareY == ownY + movementDirection;
}
