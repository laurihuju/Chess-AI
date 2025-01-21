#include "include.h"

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

	return 0;
}

void GameState::printBoard() const {
    // Set the locale to support Unicode
    std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    std::wcout << L"  +---+---+---+---+---+---+---+---+" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::wcout << 8 - i << L" |";
        for (int j = 0; j < 8; ++j) {
            wchar_t pieceChar = L' ';
            if (_board[i][j] != nullptr) {
                if (dynamic_cast<Rook*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u2656' : L'\u265C';
                else if (dynamic_cast<Knight*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u2658' : L'\u265E';
                else if (dynamic_cast<Bishop*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u2657' : L'\u265D';
                else if (dynamic_cast<Queen*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u2655' : L'\u265B';
                else if (dynamic_cast<King*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u2654' : L'\u265A';
                else if (dynamic_cast<Pawn*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u2659' : L'\u265F';
            }
            else {
                // Determine if the square is white or black
                if ((i + j) % 2 == 0) {
                    pieceChar = L'.';
                }
                else {
                    pieceChar = L'#';
                }
            }
            std::wcout << L" " << pieceChar << L" |";
        }
        std::wcout << std::endl;
        std::wcout << L"  +---+---+---+---+---+---+---+---+" << std::endl;
    }
    std::wcout << L"    a   b   c   d   e   f   g   h" << std::endl;
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

void GameState::setUpperLeftCastlingNotPossible() {
	_upperLeftCastlingPossible = false;
}
void GameState::setUpperRightCastlingNotPossible() {
	_upperRightCastlingPossible = false;
}
void GameState::setLowerLeftCastlingNotPossible() {
	_lowerLeftCastlingPossible = false;
}
void GameState::setLowerRightCastlingNotPossible() {
	_lowerRightCastlingPossible = false;
}
