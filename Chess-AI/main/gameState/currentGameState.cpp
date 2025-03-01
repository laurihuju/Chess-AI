#include "currentGameState.h"

#include "gameInfo.h"
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
    _board[0][0] = GameInfo::getInstance()->getPieceInstance(PieceType::Rook, false);
    _board[0][1] = GameInfo::getInstance()->getPieceInstance(PieceType::Knight, false);
    _board[0][2] = GameInfo::getInstance()->getPieceInstance(PieceType::Bishop, false);
    _board[0][3] = GameInfo::getInstance()->getPieceInstance(PieceType::Queen, false);
    _board[0][4] = GameInfo::getInstance()->getPieceInstance(PieceType::King, false);
    _board[0][5] = GameInfo::getInstance()->getPieceInstance(PieceType::Bishop, false);
    _board[0][6] = GameInfo::getInstance()->getPieceInstance(PieceType::Knight, false);
    _board[0][7] = GameInfo::getInstance()->getPieceInstance(PieceType::Rook, false);

    // Initialize board row 2 (index 1)
    for (int x = 0; x < 8; x++) {
        _board[1][x] = GameInfo::getInstance()->getPieceInstance(PieceType::Pawn, false);
    }

    // Initialize board rows 3-6 (index 2-5)
    for (int y = 2; y < 6; y++) {
        for (int x = 0; x < 8; x++) {
            _board[y][x] = 0;
        }
    }

    // Initialize board row 7 (index 6)
    for (int x = 0; x < 8; x++) {
        _board[6][x] = GameInfo::getInstance()->getPieceInstance(PieceType::Pawn, true);
    }

    // Initialize board row 8 (index 7)
    _board[7][0] = GameInfo::getInstance()->getPieceInstance(PieceType::Rook, true);
    _board[7][1] = GameInfo::getInstance()->getPieceInstance(PieceType::Knight, true);
    _board[7][2] = GameInfo::getInstance()->getPieceInstance(PieceType::Bishop, true);
    _board[7][3] = GameInfo::getInstance()->getPieceInstance(PieceType::Queen, true);
    _board[7][4] = GameInfo::getInstance()->getPieceInstance(PieceType::King, true);
    _board[7][5] = GameInfo::getInstance()->getPieceInstance(PieceType::Bishop, true);
    _board[7][6] = GameInfo::getInstance()->getPieceInstance(PieceType::Knight, true);
    _board[7][7] = GameInfo::getInstance()->getPieceInstance(PieceType::Rook, true);

    // Calculate the game phase value
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (_board[j][i] == 0)
                continue;

            _gamePhase += _board[j][i]->gamePhaseInfluence();
        }
    }

    // Calculate hash
    _hash = 0;
    _hash = _hash xor GameInfo::getInstance()->whiteSideToMoveZobristValue();
    _hash = _hash xor GameInfo::getInstance()->upperLeftCastlingZobristValue();
    _hash = _hash xor GameInfo::getInstance()->upperRightCastlingZobristValue();
    _hash = _hash xor GameInfo::getInstance()->lowerLeftCastlingZobristValue();
    _hash = _hash xor GameInfo::getInstance()->lowerRightCastlingZobristValue();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (_board[j][i] == 0)
                continue;

            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[j][i]->getType(), _board[j][i]->isWhite(), i, j);
        }
    }

}

CurrentGameState* CurrentGameState::getInstance() {
    return instance;
}
