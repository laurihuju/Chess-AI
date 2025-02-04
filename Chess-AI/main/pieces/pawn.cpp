#include <vector>
#include <cmath>
#include "pawn.h"
#include "../move.h"
#include "../piece.h"
#include "../gameState/gameState.h"

Pawn::Pawn(bool isWhite) : Piece(isWhite) {}

void Pawn::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Moves for white
	if (isWhite()) {
		// There are no possible moves when white pawn is on the top row
		if (y <= 0) {
			return;
		}

		// Capturing
		if (x > 0) {
			Piece* piece = gameState.getPieceAt(x - 1, y - 1);
			if (piece != 0 && !piece->isWhite()) {
				moves.push_back(Move(x, y, x - 1, y - 1));
			}
		}
		if (x < 7 && gameState.getPieceAt(x + 1, y - 1)) {
			Piece* piece = gameState.getPieceAt(x + 1, y - 1);
			if (piece != 0 && !piece->isWhite()) {
				moves.push_back(Move(x, y, x + 1, y - 1));
			}
		}

		// En passant
		if (y == 3 && gameState.upperEnPassantColumn() == x - 1) {
			moves.push_back(Move(x, y, x - 1, y - 1));
		}
		else if (y == 3 && gameState.upperEnPassantColumn() == x + 1) {
			moves.push_back(Move(x, y, x + 1, y - 1));
		}

		// Prevent moving forward when there is an obstacle
		if (gameState.getPieceAt(x, y - 1) != 0) {
			return;
		}

		// Forward moves
		if (y == 6 && gameState.getPieceAt(x, 4) == 0) {
			moves.push_back(Move(x, y, x, y - 2));
		}
		if (y > 1) {
			moves.push_back(Move(x, y, x, y - 1));
		}
		else {
			moves.push_back(Move(x, y, x, y - 1, 'q'));
			moves.push_back(Move(x, y, x, y - 1, 'n'));
			moves.push_back(Move(x, y, x, y - 1, 'b'));
			moves.push_back(Move(x, y, x, y - 1, 'r'));
		}

		return;
	}

	// Moves for black

	// There are no possible moves when black pawn is on the bottom row
	if (y >= 7) {
		return;
	}

	// Capturing
	if (x > 0) {
		Piece* piece = gameState.getPieceAt(x - 1, y + 1);
		if (piece != 0 && piece->isWhite()) {
			moves.push_back(Move(x, y, x - 1, y + 1));
		}
	}
	if (x < 7 && gameState.getPieceAt(x + 1, y + 1)) {
		Piece* piece = gameState.getPieceAt(x + 1, y + 1);
		if (piece != 0 && piece->isWhite()) {
			moves.push_back(Move(x, y, x + 1, y + 1));
		}
	}

	// En passant
	if (y == 4 && gameState.lowerEnPassantColumn() == x - 1) {
		moves.push_back(Move(x, y, x - 1, y + 1));
	}
	else if (y == 4 && gameState.lowerEnPassantColumn() == x + 1) {
		moves.push_back(Move(x, y, x + 1, y + 1));
	}

	// Prevent moving forward when there is an obstacle
	if (gameState.getPieceAt(x, y + 1) != 0) {
		return;
	}

	// Forward moves
	if (y == 1 && gameState.getPieceAt(x, 3) == 0) {
		moves.push_back(Move(x, y, x, y + 2));
	}
	if (y < 6) {
		moves.push_back(Move(x, y, x, y + 1));
	}
	else {
		moves.push_back(Move(x, y, x, y + 1, 'q'));
		moves.push_back(Move(x, y, x, y + 1, 'n'));
		moves.push_back(Move(x, y, x, y + 1, 'b'));
		moves.push_back(Move(x, y, x, y + 1, 'r'));
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
