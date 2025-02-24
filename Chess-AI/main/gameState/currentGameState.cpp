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

    // Reserve space for all pieces in the _pieces vector
    _pieces.reserve(12);

    // Create black piece instances
    Rook* blackRook = new Rook(false);
    _pieces.push_back(blackRook);
    Knight* blackKnight = new Knight(false);
    _pieces.push_back(blackKnight);
    Bishop* blackBishop = new Bishop(false);
    _pieces.push_back(blackBishop);
    Queen* blackQueen = new Queen(false);
    _pieces.push_back(blackQueen);
    King* blackKing = new King(false);
    _pieces.push_back(blackKing);
    Pawn* blackPawn = new Pawn(false);
    _pieces.push_back(blackPawn);

    // Create white piece instances
    Rook* whiteRook = new Rook(true);
    _pieces.push_back(whiteRook);
    Knight* whiteKnight = new Knight(true);
    _pieces.push_back(whiteKnight);
    Bishop* whiteBishop = new Bishop(true);
    _pieces.push_back(whiteBishop);
    Queen* whiteQueen = new Queen(true);
    _pieces.push_back(whiteQueen);
    King* whiteKing = new King(true);
    _pieces.push_back(whiteKing);
    Pawn* whitePawn = new Pawn(true);
    _pieces.push_back(whitePawn);

    // Initialize board row 1
    _board[0] = blackRook;
    _board[1] = blackKnight;
    _board[2] = blackBishop;
    _board[3] = blackQueen;
    _board[4] = blackKing;
    _board[5] = blackBishop;
    _board[6] = blackKnight;
    _board[7] = blackRook;

    // Initialize board row 2
    for (int x = 0; x < 8; x++) {
        _board[8 + x] = blackPawn;
    }

    // Initialize board rows 3-6
    for (int y = 2; y < 6; y++) {
        for (int x = 0; x < 8; x++) {
            _board[(y * 8) + x] = 0;
        }
    }

    // Initialize board row 7
    for (int x = 0; x < 8; x++) {
        _board[48 + x] = whitePawn;
    }

    // Initialize board row 8
    _board[56] = whiteRook;
    _board[57] = whiteKnight;
    _board[58] = whiteBishop;
    _board[59] = whiteQueen;
    _board[60] = whiteKing;
    _board[61] = whiteBishop;
    _board[62] = whiteKnight;
    _board[63] = whiteRook;

    // Calculate the game phase value
	_gamePhase = whiteBishop->gamePhaseInfluence() * 2 + blackBishop->gamePhaseInfluence() * 2
		+ whiteKing->gamePhaseInfluence() + blackKing->gamePhaseInfluence()
		+ whiteKnight->gamePhaseInfluence() * 2 + blackKnight->gamePhaseInfluence() * 2
		+ whitePawn->gamePhaseInfluence() * 8 + blackPawn->gamePhaseInfluence() * 8
		+ whiteQueen->gamePhaseInfluence() + blackQueen->gamePhaseInfluence()
		+ whiteRook->gamePhaseInfluence() * 2 + blackRook->gamePhaseInfluence() * 2;

}

CurrentGameState::~CurrentGameState() {
    for (int i = 0; i < _pieces.size(); i++) {
        delete _pieces[i];
    }
}

Piece* CurrentGameState::getPieceInstance(PieceType type, bool isWhite) const {
    for (int i = 0; i < _pieces.size(); i++) {
        if (_pieces[i]->getType() != type)
            continue;
        if (_pieces[i]->isWhite() != isWhite)
            continue;

        return _pieces[i];
    }

    return 0;
}

CurrentGameState* CurrentGameState::getInstance() {
    return instance;
}
