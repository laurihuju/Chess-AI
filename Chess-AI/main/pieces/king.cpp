#include <vector>
#include <cmath>
#include "king.h"
#include "../move.h"
#include "../gameState/gameState.h"

/// <summary>
/// The additions and reductions of the value of white king at different positions in the middle game.
/// </summary>
int middleKingValueAdditions[8][8] =
{
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-20, -30, -30, -40, -40, -30, -30, -20},
	{-10, -20, -20, -20, -20, -20, -20, -10},
	{20, 20, 0, 0, 0, 0, 20, 20},
	{20, 30, 10, 0, 0, 10, 30, 20}
};

/// <summary>
/// The additions and reductions of the value of white king at different positions in the end game.
/// </summary>
int endKingValueAdditions[8][8] =
{
	{-50, -40, -30, -20, -20, -30, -40, -50},
	{-30, -20, -10, 0, 0, -10, -20, -30},
	{-30, -10, 20, 30, 30, 20, -10, -30},
	{-30, -10, 30, 40, 40, 30, -10, -30},
	{-30, -10, 30, 40, 40, 30, -10, -30},
	{-30, -10, 20, 30, 30, 20, -10, -30},
	{-30, -30, 0, 0, 0, 0, -30, -30},
	{-50, -30, -30, -30, -30, -30, -30, -50}
};

King::King(bool isWhite) : Piece(isWhite) {}

PieceType King::getType() const {
	return PieceType::King;
}

char King::gamePhaseInfluence() const {
	return 0;
}

void King::possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly) const {
	// Directions: up, down, left, right
	char directions[8][2] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },{ -1,-1 },{ -1,1 },{ 1,-1 },{ 1,1 } };
	for (auto& dir : directions) {
		char dx = x + dir[0];
		char dy = y + dir[1];
		if (dx < 0 || dx > 7 || dy < 0 || dy > 7)
			continue;

		Piece* p = gameState.getPieceAt(dx, dy);
		if (p) {
			if (p->isWhite() != this->isWhite()) {
				moves.push_back(Move(x, y, dx, dy));
			}
			continue;
		}
		
		if (!captureOnly) {
			moves.push_back(Move(x, y, dx, dy));
		}
	}

	// Castling
	if (!captureOnly && !gameState.isCheck(isWhite())) {
		char castlingRow = isWhite() ? 7 : 0;

		// Left castling
		if ((isWhite() ? gameState.lowerLeftCastlingPossible() : gameState.upperLeftCastlingPossible()) && gameState.getPieceAt(1, castlingRow) == 0 && gameState.getPieceAt(2, castlingRow) == 0 && gameState.getPieceAt(3, castlingRow) == 0 && !gameState.isThreatened(isWhite(), 2, castlingRow) && !gameState.isThreatened(isWhite(), 3, castlingRow)) {
			moves.push_back(Move(4, castlingRow, 2, castlingRow));
		}

		// Right castling
		if ((isWhite() ? gameState.lowerRightCastlingPossible() : gameState.upperRightCastlingPossible()) && gameState.getPieceAt(5, castlingRow) == 0 && gameState.getPieceAt(6, castlingRow) == 0 && !gameState.isThreatened(isWhite(), 5, castlingRow) && !gameState.isThreatened(isWhite(), 6, castlingRow)) {
			moves.push_back(Move(4, castlingRow, 6, castlingRow));
		}
	}

}

int King::evaluationValue(char x, char y, char gamePhase) const {
	return gamePhase > 10 ? middleKingValueAdditions[isWhite() ? y : 7 - y][x] : endKingValueAdditions[isWhite() ? y : 7 - y][x];
}
