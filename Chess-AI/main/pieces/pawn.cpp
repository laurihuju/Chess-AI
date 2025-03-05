#include <vector>
#include <cmath>
#include "pawn.h"
#include "../move.h"
#include "../piece.h"
#include "../gameState/gameState.h"

/// <summary>
/// The additions and reductions of the value of white pawn at different positions.
/// </summary>
int pawnValueAdditions[8][8] =
{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{50, 50, 50, 50, 50, 50, 50, 50},
	{10, 10, 20, 30, 30, 20, 10, 10},
	{5, 5, 10, 25, 25, 10, 5, 5},
	{0, 0, 0, 20, 20, 0, 0, 0},
	{5, -5, -10, 0, 0, -10, -5, 5},
	{5, 10, 10, -20, -20, 10, 10, 5},
	{0, 0, 0, 0, 0, 0, 0, 0}
};

Pawn::Pawn(bool isWhite) : Piece(isWhite) {}

PieceType Pawn::getType() const {
	return PieceType::Pawn;
}

char Pawn::gamePhaseInfluence() const {
	return 0;
}

void Pawn::possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly) const {
	// There are no possible moves when the pawn is at the top or bottom row
	if (y == 0 || y == 7) {
		return;
	}
	
	// The possible movement direction (white moves up and black down)
	char movementDirection = isWhite() ? -1 : 1;

	// Capturing
	for (char i = (x - 1 > 0 ? x - 1 : 0); i <= x + 1 && i <= 7; i++) {
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
	if (gameState.upperEnPassantColumn() != -1 && movementDirection == -1 && y == 3 && std::abs(gameState.upperEnPassantColumn() - x) == 1) {
		moves.push_back(Move(x, y, gameState.upperEnPassantColumn(), 2));
	} else if (gameState.lowerEnPassantColumn() != -1 && movementDirection == 1 && y == 4 && std::abs(gameState.lowerEnPassantColumn() - x) == 1) {
		moves.push_back(Move(x, y, gameState.lowerEnPassantColumn(), 5));
	}

	// Don't generate normal moves if only capture moves are needed
	if (captureOnly) {
		return;
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

int Pawn::evaluationValue(char x, char y, char gamePhase) const {
	return 100 + pawnValueAdditions[isWhite() ? y : 7 - y][x];
}
