#include "GameInfo.h"

#include <random>
#include "../pieces/bishop.h"
#include "../pieces/king.h"
#include "../pieces/knight.h"
#include "../pieces/pawn.h"
#include "../pieces/queen.h"
#include "../pieces/rook.h"

GameInfo* GameInfo::instance;

GameInfo::GameInfo() {
    if (instance == 0) {
        instance = this;
    }

    // Reserve space for all pieces in the _pieces vector
    _pieces.reserve(12);

    // Create black piece instances
    _pieces.push_back(new Rook(false));
    _pieces.push_back(new Knight(false));
    _pieces.push_back(new Bishop(false));
    _pieces.push_back(new Queen(false));
    _pieces.push_back(new King(false));
    _pieces.push_back(new Pawn(false));

    // Create white piece instances
    _pieces.push_back(new Rook(true));
    _pieces.push_back(new Knight(true));
    _pieces.push_back(new Bishop(true));
    _pieces.push_back(new Queen(true));
    _pieces.push_back(new King(true));
    _pieces.push_back(new Pawn(true));

    // Create random number generator for zobrist values
    std::random_device randomDevice;
    std::mt19937_64 randomNumberGenerator(randomDevice());
    std::uniform_int_distribution<uint64_t> randomNumberDistribution(0, UINT64_MAX);

    // Generate piece zobrist values
    for (int pieceType = 0; pieceType < 12; pieceType++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                _pieceZobristValues[pieceType][y][x] = randomNumberDistribution(randomNumberGenerator);
            }
        }
    }

    // Generate white side to move zobrist value
    _whiteSideToMoveZobristValue = randomNumberDistribution(randomNumberGenerator);

    // Generate castling flag zobrist values
    _upperLeftCastlingZobristValue = randomNumberDistribution(randomNumberGenerator);
    _upperRightCastlingZobristValue = randomNumberDistribution(randomNumberGenerator);
    _lowerLeftCastlingZobristValue = randomNumberDistribution(randomNumberGenerator);
    _lowerRightCastlingZobristValue = randomNumberDistribution(randomNumberGenerator);

    // Generate en passant flag zobrist values
    for (int i = 0; i < 8; i++) {
        _upperEnPassantZobristValues[i] = randomNumberDistribution(randomNumberGenerator);
        _lowerEnPassantZobristValues[i] = randomNumberDistribution(randomNumberGenerator);
    }
}

GameInfo::~GameInfo() {
    instance = 0;

    for (int i = 0; i < _pieces.size(); i++) {
        delete _pieces[i];
    }
}

Piece* GameInfo::getPieceInstance(PieceType type, bool isWhite) const {
    for (int i = 0; i < _pieces.size(); i++) {
        if (_pieces[i]->getType() != type)
            continue;
        if (_pieces[i]->isWhite() != isWhite)
            continue;

        return _pieces[i];
    }

    return 0;
}

uint64_t GameInfo::pieceZobristValue(PieceType type, bool isWhite, char x, char y) {
    return _pieceZobristValues[(isWhite ? 0 : 6) + static_cast<int>(type)][y][x];
}

uint64_t GameInfo::whiteSideToMoveZobristValue() const {
    return _whiteSideToMoveZobristValue;
}

uint64_t GameInfo::upperLeftCastlingZobristValue() const {
    return _upperLeftCastlingZobristValue;
}

uint64_t GameInfo::upperRightCastlingZobristValue() const {
    return _upperRightCastlingZobristValue;
}

uint64_t GameInfo::lowerLeftCastlingZobristValue() const {
    return _lowerLeftCastlingZobristValue;
}

uint64_t GameInfo::lowerRightCastlingZobristValue() const {
    return _lowerRightCastlingZobristValue;
}

uint64_t GameInfo::upperEnPassantZobristValue(int x) const {
    return _upperEnPassantZobristValues[x];
}

uint64_t GameInfo::lowerEnPassantZobristValue(int x) const {
    return _lowerEnPassantZobristValues[x];
}

GameInfo* GameInfo::getInstance() {
    return instance;
}
