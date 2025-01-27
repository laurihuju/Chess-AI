#include <vector>
#include "pawn.h"
#include "../move.h"
#include "../gameState.h"
#include "../piece.h"

Pawn::Pawn(bool isWhite) : Piece(isWhite) {}

std::vector<Move> Pawn::possibleMoves(int x, int y, const GameState& gameState) const {
	std::vector<Move> moves;

	// Moves for white
	if (isWhite()) {
		// There are no possible moves when white pawn is on the top row
		if (y <= 0) {
			return moves;
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
		} else if (y == 3 && gameState.upperEnPassantColumn() == x + 1) {
			moves.push_back(Move(x, y, x + 1, y - 1));
		}

		// Prevent moving forward when there is an obstacle
		if (gameState.getPieceAt(x, y - 1) != 0) {
			return moves;
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
			moves.push_back(Move(x, y, x, y - 1, 'k'));
			moves.push_back(Move(x, y, x, y - 1, 'b'));
			moves.push_back(Move(x, y, x, y - 1, 'r'));
		}

		return moves;
	}

	// Moves for black

	// There are no possible moves when black pawn is on the bottom row
	if (y >= 7) {
		return moves;
	}

	// Capturing
	if (x > 0) {
		Piece* piece = gameState.getPieceAt(x - 1, y + 1);
		if (piece != 0 && piece->isWhite()) {
			moves.push_back(Move(x, y, x - 1, y + 1));
		}
	}
	if (x < 7 && gameState.getPieceAt(x + 1, y - 1)) {
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
		return moves;
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
		moves.push_back(Move(x, y, x, y + 1, 'k'));
		moves.push_back(Move(x, y, x, y + 1, 'b'));
		moves.push_back(Move(x, y, x, y + 1, 'r'));
	}

	return moves;
}
