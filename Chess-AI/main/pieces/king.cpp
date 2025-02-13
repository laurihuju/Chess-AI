#include <vector>
#include <cmath>
#include "king.h"
#include "../move.h"
#include "../gameState/gameState.h"
#include "queen.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"

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

void King::possibleMoves(std::vector<Move>& moves, int x, int y, const GameState& gameState) const {
	// Directions: up, down, left, right
	int directions[8][2] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },{ -1,-1 },{ -1,1 },{ 1,-1 },{ 1,1 } };
	for (auto& dir : directions) {
		int dx = x + dir[0];
		int dy = y + dir[1];
		if (dx < 0 || dx > 7 || dy < 0 || dy > 7)
			continue;
		Piece* p = gameState.getPieceAt(dx, dy);
		if (p) {
			if (p->isWhite() != this->isWhite()) {
				moves.push_back(Move(x, y, dx, dy));
			}
		}
		else {
			moves.push_back(Move(x, y, dx, dy));
		}
	}

	// Castling
	if (!gameState.isCheck(isWhite())) {
		int castlingRow = isWhite() ? 7 : 0;

		// Left castling
		if ((isWhite() ? gameState.lowerLeftCastlingPossible() : gameState.upperLeftCastlingPossible()) && gameState.getPieceAt(1, castlingRow) == 0 && gameState.getPieceAt(2, castlingRow) == 0 && gameState.getPieceAt(3, castlingRow) == 0 && !gameState.isThreatened(isWhite(), 2, castlingRow) && !gameState.isThreatened(isWhite(), 3, castlingRow)) {
			moves.push_back(Move(4, 7, 2, 7));
		}

		// Right castling
		if ((isWhite() ? gameState.lowerRightCastlingPossible() : gameState.upperRightCastlingPossible()) && gameState.getPieceAt(5, castlingRow) == 0 && gameState.getPieceAt(6, castlingRow) == 0 && !gameState.isThreatened(true, 5, castlingRow) && !gameState.isThreatened(isWhite(), 6, castlingRow)) {
			moves.push_back(Move(4, 7, 6, 7));
		}
	}

}

bool King::threatensSquare(int ownX, int ownY, int squareX, int squareY, const GameState& gameState) const {
	return std::abs(ownX - squareX) <= 1 && std::abs(ownY - squareY) <= 1;
}

Piece* King::clone() const {
	return new King(isWhite());
}

int King::evaluationValue(const GameState& gameState, int x, int y) const {
	int whitePieces = 0; // White pieces (other than pawns)
	int whiteMinorPieces = 0; // White minor pieces
	bool whiteQueenExists = false; // If white queen exists
	int blackPieces = 0; // Black pieces (other than pawns)
	int blackMinorPieces = 0; // Black minor pieces
	bool blackQueenExists = false; // If black queen exists

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Piece* currentPiece = gameState.getPieceAt(i, j);
			if (currentPiece == 0 || dynamic_cast<Pawn*>(currentPiece) != 0) {
				continue;
			}

			bool isQueen = dynamic_cast<Queen*>(currentPiece) != 0;
			bool isMinorPiece = dynamic_cast<Bishop*>(currentPiece) != 0 || dynamic_cast<Knight*>(currentPiece) != 0;

			if (currentPiece->isWhite()) {
				if (isQueen) {
					whiteQueenExists = true;
				}
				else if (isMinorPiece) {
					whiteMinorPieces++;
				}

				whitePieces++;
			}
			else {
				if (isQueen) {
					blackQueenExists = true;
				}
				else if (isMinorPiece) {
					blackMinorPieces++;
				}

				blackPieces++;
			}

		}
	}

	bool endGame = (!whiteQueenExists || whitePieces == 0 || (whitePieces == 1 && whiteMinorPieces == 1)) && (!blackQueenExists || blackPieces == 0 || (blackPieces == 1 && blackMinorPieces == 1));

	return !endGame ? middleKingValueAdditions[isWhite() ? y : 7 - y][x] : endKingValueAdditions[isWhite() ? y : 7 - y][x];
}
