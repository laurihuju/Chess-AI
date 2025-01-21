#include "gameState.h"

GameState::GameState() {}

GameState::~GameState() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (_board[i][j] == 0)
				continue;

			delete _board[i][j];
		}
	}

}

King* GameState::findKing(bool isWhite, int& x, int& y) const {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			King* king = dynamic_cast<King*>(_board[i][j]);
			if (king == 0)
				continue;
			if (king->isWhite() != isWhite)
				continue;

			x = i;
			y = j;

			return king;
		}
	}

}

// Castling
bool GameState::upperLeftCastlingPossible() const {
	return _upperLeftCastlingPossible;
}
bool GameState::upperRightCastlingPossible() const {
	return _upperRightCastlingPossible;
}
bool GameState::lowerLeftCastlingPossible() const {
	return _lowerLeftCastlingPossible;
}
bool GameState::lowerRightCastlingPossible() const {
	return _lowerRightCastlingPossible;
}

bool GameState::setUpperLeftCastlingNotPossible() {
	_upperLeftCastlingPossible = false;
}
bool GameState::setUpperRightCastlingNotPossible() {
	_upperRightCastlingPossible = false;
}
bool GameState::setLowerLeftCastlingNotPossible() {
	_lowerLeftCastlingPossible = false;
}
bool GameState::setLowerRightCastlingNotPossible() {
	_lowerRightCastlingPossible = false;
}
