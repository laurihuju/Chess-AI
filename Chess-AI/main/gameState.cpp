#include <locale>
#include <codecvt>
#include <iostream>
#include "gameState.h"
#include "move.h"

#include "pieces/rook.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/queen.h"
#include "pieces/king.h"
#include "pieces/pawn.h"

GameState::GameState() {
    // Initialize board row 1 (index 0)
    _board[0][0] = new Rook(false);
    _board[0][1] = new Knight(false);
    _board[0][2] = new Bishop(false);
    _board[0][3] = new Queen(false);
    _board[0][4] = new King(false);
    _board[0][5] = new Bishop(false);
    _board[0][6] = new Knight(false);
    _board[0][7] = new Rook(false);

    // Initialize board row 2 (index 1)
    for (int x = 0; x < 8; x++) {
        _board[1][x] = new Pawn(false);
    }

    // Initialize board rows 3-6 (index 2-5)
    for (int y = 2; y < 6; y++) {
        for (int x = 0; x < 8; x++) {
            _board[y][x] = 0;
        }
    }

    // Initialize board row 7 (index 6)
    for (int x = 0; x < 8; x++) {
        _board[6][x] = new Pawn(true);
    }

    // Initialize board row 8 (index 7)
    _board[7][0] = new Rook(true);
    _board[7][1] = new Knight(true);
    _board[7][2] = new Bishop(true);
    _board[7][3] = new Queen(true);
    _board[7][4] = new King(true);
    _board[7][5] = new Bishop(true);
    _board[7][6] = new Knight(true);
    _board[7][7] = new Rook(true);

}

GameState::~GameState() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (_board[i][j] == 0)
				continue;

			delete _board[i][j];
		}
	}

}

bool GameState::applyMove(const Move& move) {
    if (_board[move.x2()][move.y2()] != 0)
        return false;

    _board[move.x2()][move.y2()] = _board[move.x1()][move.y1()];
    _board[move.x1()][move.y1()] = 0;

    return true;
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

    std::wcout << L"    a   b   c   d   e   f   g   h" << std::endl;
    std::wcout << L"  +---+---+---+---+---+---+---+---+" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::wcout << 8 - i << L" |";
        for (int j = 0; j < 8; ++j) {
            wchar_t pieceChar = L' ';
            if (_board[i][j] != nullptr) {
                if (dynamic_cast<Rook*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265C' : L'\u2656';
                else if (dynamic_cast<Knight*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265E' : L'\u2658';
                else if (dynamic_cast<Bishop*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265D' : L'\u2657';
                else if (dynamic_cast<Queen*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265B' : L'\u2655';
                else if (dynamic_cast<King*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265A' : L'\u2654';
                else if (dynamic_cast<Pawn*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265F' : L'\u2659';
            }
            else {
                // Determine if the square is white or black
                if ((i + j) % 2 == 0) {
                    pieceChar = L'\u25A0'; // Black square
                }
                else {
                    pieceChar = L'\u25A1'; // White square
                }
            }
            std::wcout << L" " << pieceChar << L" |";
        }
        std::wcout << L" " << 8 - i << std::endl;
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
