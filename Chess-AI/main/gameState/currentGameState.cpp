#include "currentGameState.h"

#include "../pieces/bishop.h"
#include "../pieces/king.h"
#include "../pieces/knight.h"
#include "../pieces/pawn.h"
#include "../pieces/queen.h"
#include "../pieces/rook.h"

CurrentGameState* CurrentGameState::instance;

CurrentGameState::CurrentGameState() {
    instance = this;

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

CurrentGameState* CurrentGameState::getInstance() {
    return instance;
}
