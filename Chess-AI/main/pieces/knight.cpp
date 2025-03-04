#include <vector>
#include <cmath>
#include "knight.h"
#include "../move.h"
#include "../gameState/gameState.h"

/// <summary>
/// The additions and reductions of the value of white knight at different positions.
/// </summary>
int knightValueAdditions[8][8] =
{
	{-50, -40, -30, -30, -30, -30, -40, -50},
	{-40, -20, 0, 0, 0, 0, -20, -40},
	{-30, 0, 10, 15, 15, 10, 0, -30},
	{-30, 5, 15, 20, 20, 15, 5, -30},
	{-30, 0, 15, 20, 20, 15, 0, -30},
	{-30, 5, 10, 15, 15, 10, 5, -30},
	{-40, -20, 0, 5, 5, 0, -20, -40},
	{-50, -40, -30, -30, -30, -30, -40, -50}
};

Knight::Knight(bool isWhite) : Piece(isWhite) {}

PieceType Knight::getType() const {
	return PieceType::Knight;
}

char Knight::gamePhaseInfluence() const {
	return 1;
}

void Knight::possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly) const {
	// Possible moves for a knight
	char directions[8][2] = { {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };
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

}

bool Knight::threatensSquare(char ownX, char ownY, char squareX, char squareY, const GameState& gameState) const {
	return (std::abs(ownX - squareX) == 2 && std::abs(ownY - squareY) == 1) || std::abs(ownX - squareX) == 1 && std::abs(ownY - squareY) == 2;
}

int Knight::evaluationValue(const GameState& gameState, char x, char y) const {
	return 300 + knightValueAdditions[isWhite() ? y : 7 - y][x];
}
