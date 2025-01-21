#include "gameState.h"
#include <iostream>

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

void GameState::printBoard() const {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (_board[i][j] == 0) {
				if ((i + j) % 2 == 0) {
					// white square
					std::cout << " # ";
				}
				else {
					// black square
					std::cout << "   ";
				}
			}
			else {
				char pieceChar = ' ';
				if (dynamic_cast<Rook*>(_board[i][j])) pieceChar = 'R';
				else if (dynamic_cast<Knight*>(_board[i][j])) pieceChar = 'N';
				else if (dynamic_cast<Bishop*>(_board[i][j])) pieceChar = 'B';
				else if (dynamic_cast<Queen*>(_board[i][j])) pieceChar = 'Q';
				else if (dynamic_cast<King*>(_board[i][j])) pieceChar = 'K';
				else if (dynamic_cast<Pawn*>(_board[i][j])) pieceChar = 'P';

				if (_board[i][j]->isWhite()) {
					pieceChar = tolower(pieceChar);
				}
				std::cout << " " << pieceChar << " ";
			}
		}
		std::cout << std::endl;
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
