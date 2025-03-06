#include <vector>
#include <cmath>
#include "bishop.h"
#include "../move.h"
#include "../gameState/gameState.h"

/// <summary>
/// The additions and reductions of the value of white bishop at different positions.
/// </summary>
int bishopValueAdditions[8][8] =
{
	{-20, -10, -10, -10, -10, -10, -10, -20},
	{-10, 0, 0, 0, 0, 0, 0, -10},
	{-10, 0, 5, 10, 10, 5, 0, -10},
	{-10, 5, 5, 10, 10, 5, 5, -10},
	{-10, 0, 10, 10, 10, 10, 0, -10},
	{-10, 10, 10, 10, 10, 10, 10, -10},
	{-10, 5, 0, 0, 0, 0, 5, -10},
	{-20, -10, -10, -10, -10, -10, -10, -20}
};

Bishop::Bishop(bool isWhite) : Piece(isWhite) {}

PieceType Bishop::getType() const {
	return PieceType::Bishop;
}

char Bishop::gamePhaseInfluence() const {
	return 1;
}

void Bishop::possibleMoves(std::vector<Move>& moves, char x, char y, const GameState& gameState, bool captureOnly) const {
	// Directions: top-right, top-left, bottom-right, bottom-left
	char directions[4][2] = { {-1,1}, {-1,-1}, {1,1}, {1,-1} };
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

int Bishop::evaluationValue(char x, char y, char gamePhase) const {
	return 300 + bishopValueAdditions[isWhite() ? y : 7 - y][x];
}
