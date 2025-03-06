#include <vector>
#include "rook.h"
#include "../move.h"
#include "../gameState/gameState.h"

/// <summary>
/// The additions and reductions of the value of white rook at different positions.
/// </summary>
int rookValueAdditions[8][8] =
{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{5, 10, 10, 10, 10, 10, 10, 5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{-5, 0, 0, 0, 0, 0, 0, -5},
	{0, 0, 0, 5, 5, 0, 0, 0}
};

Rook::Rook(bool isWhite) : Piece(isWhite) {}

PieceType Rook::getType() const {
	return PieceType::Rook;
}

char Rook::gamePhaseInfluence() const {
	return 2;
}

void Rook::possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly) const {
	// Directions: up, down, left, right
	char directions[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
	for (auto& dir : directions) {
		char dx = x;
		char dy = y;
		while (true) {
			dx += dir[0];
			dy += dir[1];
			if (dx < 0 || dx > 7 || dy < 0 || dy > 7)
				break;

			Piece* p = gameState.getPieceAt(dx, dy);
			if (p) {
				if (p->isWhite() != this->isWhite()) {
					moves.push_back(Move(x, y, dx, dy));
				}
				break;
			}
			
			if (!captureOnly) {
				moves.push_back(Move(x, y, dx, dy));
			}
		}
	}
}

int Rook::evaluationValue(char x, char y, char gamePhase) const {
	return 500 + rookValueAdditions[isWhite() ? y : 7 - y][x];
}
