#include <locale>
#include <codecvt>
#include <iostream>
#include "gameState.h"
#include "currentGameState.h"
#include "../move.h"

#include "../pieces/rook.h"
#include "../pieces/knight.h"
#include "../pieces/bishop.h"
#include "../pieces/queen.h"
#include "../pieces/king.h"
#include "../pieces/pawn.h"

bool GameState::operator==(const GameState& other) const {
    if (_upperLeftCastlingPossible != other._upperLeftCastlingPossible)
        return false;
    if (_upperRightCastlingPossible != other._upperRightCastlingPossible)
        return false;
    if (_lowerLeftCastlingPossible != other._lowerLeftCastlingPossible)
        return false;
    if (_lowerRightCastlingPossible != other._lowerRightCastlingPossible)
        return false;
    if (_upperEnPassantColumn != other._upperEnPassantColumn)
        return false;
    if (_lowerEnPassantColumn != other._lowerEnPassantColumn)
        return false;

    for (int i = 0; i < 64; i++) {
        if (_board[i] == 0 && other._board[i] == 0)
            continue;
        if (_board[i] == 0 || other._board[i] == 0)
            return false;
        if (*_board[i] != *other._board[i])
            return false;
    }

    return true;
}

bool GameState::operator!=(const GameState& other) const {
    return !(*this == other);
}

GameState::GameState() {
    // Board content
    for (int i = 0; i < 64; i++) {
        _board[i] = 0;
    }

    // Game phase
    _gamePhase = 0;

}

GameState::~GameState() {}

void GameState::applyMove(const Move& move) {
    int startSquareIndex = (move.y1() * 8) + move.x1();
    int endSquareIndex = (move.y2() * 8) + move.x2();

    // Handle the move and capturing
    if (_board[endSquareIndex] != 0)
        _gamePhase -= _board[endSquareIndex]->gamePhaseInfluence();
    _board[endSquareIndex] = _board[startSquareIndex];
    _board[startSquareIndex] = 0;

    // Handle promotion
    if (move.promotionPiece() != -1) {
        PieceType promotionPieceType;
		switch (move.promotionPiece()) {
		case 'n':
			promotionPieceType = PieceType::Knight;
			break;
        case 'b':
            promotionPieceType = PieceType::Bishop;
            break;
        case 'r':
            promotionPieceType = PieceType::Rook;
            break;
        default:
            promotionPieceType = PieceType::Queen;
            break;
		}

        Piece* promotionPiece = CurrentGameState::getInstance()->getPieceInstance(promotionPieceType, _board[endSquareIndex]->isWhite());
        _gamePhase -= _board[endSquareIndex]->gamePhaseInfluence();
        _board[endSquareIndex] = promotionPiece;
        _gamePhase += _board[endSquareIndex]->gamePhaseInfluence();
    }

    // Handle en passant move
    if (_board[endSquareIndex]->getType() == PieceType::Pawn) {
        if (move.y2() == 2 && upperEnPassantColumn() == move.x2()) {
            if (_board[24 + move.x2()] != 0)
                _gamePhase -= _board[24 + move.x2()]->gamePhaseInfluence();
            _board[24 + move.x2()] = 0;
        } else if (move.y2() == 5 && upperEnPassantColumn() == move.x2()) {
            if (_board[32 + move.x2()] != 0)
                _gamePhase -= _board[32 + move.x2()]->gamePhaseInfluence();
            _board[32 + move.x2()] = 0;
        }
    }

    // Handle castling move
    if (_board[endSquareIndex]->getType() == PieceType::King) {
        if (move.x2() == 2 && move.y2() == 0 && upperLeftCastlingPossible()) {
            if (_board[3] != 0)
                _gamePhase -= _board[3]->gamePhaseInfluence();

            _board[3] = _board[0];
            _board[0] = 0;

        }
        else if (move.x2() == 6 && move.y2() == 0 && upperRightCastlingPossible()) {
            if (_board[5] != 0)
                _gamePhase -= _board[5]->gamePhaseInfluence();

            _board[5] = _board[7];
            _board[7] = 0;

        }
        else if (move.x2() == 2 && move.y2() == 7 && lowerLeftCastlingPossible()) {
            if (_board[59] != 0)
                _gamePhase -= _board[59]->gamePhaseInfluence();

            _board[59] = _board[56];
            _board[56] = 0;

        }
        else if (move.x2() == 6 && move.y2() == 7 && lowerRightCastlingPossible()) {
            if (_board[61] != 0)
                _gamePhase -= _board[61]->gamePhaseInfluence();

            _board[61] = _board[63];
            _board[63] = 0;

        }
    }

    // Update en passant flags
    _upperEnPassantColumn = -1;
    _lowerEnPassantColumn = -1;

    if (_board[endSquareIndex]->getType() == PieceType::Pawn) {
        if (move.y2() == 3 && move.y2() - move.y1() == 2) {
            _upperEnPassantColumn = move.x2();
        }
        else if (move.y2() == 4 && move.y2() - move.y1() == -2) {
            _lowerEnPassantColumn = move.x2();
        }
    }

    // Update castling flags
    if ((move.x1() == 0 && move.y1() == 0) || (move.x2() == 0 && move.y2() == 0)) {
        _upperLeftCastlingPossible = false;
    }
    else if ((move.x1() == 7 && move.y1() == 0) || (move.x2() == 7 && move.y2() == 0)) {
        _upperRightCastlingPossible = false;
    }
    else if ((move.x1() == 0 && move.y1() == 7) || (move.x2() == 0 && move.y2() == 7)) {
        _lowerLeftCastlingPossible = false;
    }
    else if ((move.x1() == 7 && move.y1() == 7) || (move.x2() == 7 && move.y2() == 7)) {
        _lowerRightCastlingPossible = false;
    }

    if (move.x1() == 4 && move.y1() == 0) {
        _upperLeftCastlingPossible = false;
        _upperRightCastlingPossible = false;
    }
    else if (move.x1() == 4 && move.y1() == 7) {
        _lowerLeftCastlingPossible = false;
        _lowerRightCastlingPossible = false;
    }

}

void GameState::findKing(bool isWhite, int& x, int& y) const {
    for (int i = 0; i < 64; i++) {
        if (_board[i] == 0)
            continue;
        if (_board[i]->getType() != PieceType::King)
            continue;
        if (_board[i]->isWhite() != isWhite)
            continue;

        x = i - ((i / 8) * 8);
        y = i / 8;

        return;
    }

}

Piece* GameState::getPieceAt(int x, int y) const {
    return _board[(y * 8) + x];
}

void GameState::possibleNewGameStates(std::vector<GameState>& newGameStates, bool isWhite) const {
    std::vector<Move> moves;

    for (int i = 0; i < 64; i++) {
        if (_board[i] == 0)
            continue;
        if (_board[i]->isWhite() != isWhite)
            continue;

        _board[i]->possibleMoves(moves, i - ((i / 8) * 8), i / 8, *this);
    }

    newGameStates.reserve(moves.size());
    for (int i = 0; i < moves.size(); i++) {
        newGameStates.emplace_back(*this);
        newGameStates.back().applyMove(moves[i]);

        if (newGameStates.back().isCheck(isWhite)) {
            newGameStates.pop_back();
        }
    }
    
}

bool GameState::isCheck(bool isWhite) const {
    int kingX;
    int kingY;
    findKing(isWhite, kingX, kingY);

    return isThreatened(isWhite, kingX, kingY);
}

bool GameState::isThreatened(bool isWhite, int x, int y) const {
    for (int i = 0; i < 64; i++) {
        if (_board[i] == 0)
            continue;
        if (_board[i]->isWhite() == isWhite)
            continue;
        if (!_board[i]->threatensSquare(i - ((i / 8) * 8), i / 8, x, y, *this))
            continue;

        return true;
    }

    return false;
}

int GameState::evaluate(bool isWhite) const {
    int evaluationValue = 0;

    for (int i = 0; i < 64; i++) {
        if (_board[i] == 0)
            continue;

        evaluationValue += _board[i]->evaluationValue(*this, i - ((i / 8) * 8), i / 8) * (_board[i]->isWhite() == isWhite ? 1 : -1);
    }

    return evaluationValue;
}

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

int GameState::upperEnPassantColumn() const {
    return _upperEnPassantColumn;
}

int GameState::lowerEnPassantColumn() const {
    return _lowerEnPassantColumn;
}

char GameState::gamePhase() const {
    return _gamePhase;
}
