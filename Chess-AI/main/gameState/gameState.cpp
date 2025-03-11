#include <locale>
#include <codecvt>
#include <iostream>
#include "gameState.h"
#include "gameInfo.h"
#include "../move.h"

#include "../pieces/rook.h"
#include "../pieces/knight.h"
#include "../pieces/bishop.h"
#include "../pieces/queen.h"
#include "../pieces/king.h"
#include "../pieces/pawn.h"

bool GameState::operator==(const GameState& other) const {
    if (_isWhiteSideToMove != other._isWhiteSideToMove)
        return false;
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

    for (char i = 0; i < 8; i++)
        for (char j = 0; j < 8; j++)
            if (_board[i][j] != other._board[i][j])
                return false;

    return true;
}

bool GameState::operator!=(const GameState& other) const {
    return !(*this == other);
}

GameState::GameState() {
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
    for (char x = 0; x < 8; x++) {
        _board[1][x] = GameInfo::getInstance()->getPieceInstance(PieceType::Pawn, false);
    }

    // Initialize board rows 3-6 (index 2-5)
    for (char y = 2; y < 6; y++) {
        for (char x = 0; x < 8; x++) {
            _board[y][x] = 0;
        }
    }

    // Initialize board row 7 (index 6)
    for (char x = 0; x < 8; x++) {
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
    for (char i = 0; i < 8; i++) {
        for (char j = 0; j < 8; j++) {
            if (_board[j][i] == 0)
                continue;

            _gamePhase += _board[j][i]->gamePhaseInfluence();
        }
    }

    // Calculate hash
    _hash = _hash xor GameInfo::getInstance()->whiteSideToMoveZobristValue();
    _hash = _hash xor GameInfo::getInstance()->upperLeftCastlingZobristValue();
    _hash = _hash xor GameInfo::getInstance()->upperRightCastlingZobristValue();
    _hash = _hash xor GameInfo::getInstance()->lowerLeftCastlingZobristValue();
    _hash = _hash xor GameInfo::getInstance()->lowerRightCastlingZobristValue();

    for (char i = 0; i < 8; i++) {
        for (char j = 0; j < 8; j++) {
            if (_board[j][i] == 0)
                continue;

            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[j][i]->getType(), _board[j][i]->isWhite(), i, j);
        }
    }

	// Calculate evaluation value
    _evaluationValue = 0;
    for (char i = 0; i < 8; i++) {
        for (char j = 0; j < 8; j++) {
            if (_board[i][j] == 0)
                continue;

            _evaluationValue += _board[i][j]->evaluationValue(j, i, _gamePhase) * (_board[i][j]->isWhite() ? 1 : -1);
        }
    }
}

void GameState::applyMove(const Move& move) {
    // Update the last move
	_lastMove = move;

    // Change the side to move
    _isWhiteSideToMove = !_isWhiteSideToMove;
    _hash = _hash xor GameInfo::getInstance()->whiteSideToMoveZobristValue();

    // Handle the move and capturing
    if (_board[move.y2()][move.x2()] != 0) {
        _gamePhase -= _board[move.y2()][move.x2()]->gamePhaseInfluence();
        _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[move.y2()][move.x2()]->getType(), _board[move.y2()][move.x2()]->isWhite(), move.x2(), move.y2());
        _evaluationValue -= _board[move.y2()][move.x2()]->evaluationValue(move.x2(), move.y2(), _gamePhase) * (_board[move.y2()][move.x2()]->isWhite() ? 1 : -1);
    }

    _board[move.y2()][move.x2()] = _board[move.y1()][move.x1()];
    _board[move.y1()][move.x1()] = 0;

    _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[move.y2()][move.x2()]->getType(), _board[move.y2()][move.x2()]->isWhite(), move.x1(), move.y1());
    _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[move.y2()][move.x2()]->getType(), _board[move.y2()][move.x2()]->isWhite(), move.x2(), move.y2());
    _evaluationValue -= _board[move.y2()][move.x2()]->evaluationValue(move.x1(), move.y1(), _gamePhase) * (_board[move.y2()][move.x2()]->isWhite() ? 1 : -1);
    _evaluationValue += _board[move.y2()][move.x2()]->evaluationValue(move.x2(), move.y2(), _gamePhase) * (_board[move.y2()][move.x2()]->isWhite() ? 1 : -1);

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

        Piece* promotionPiece = GameInfo::getInstance()->getPieceInstance(promotionPieceType, _board[move.y2()][move.x2()]->isWhite());

        _gamePhase -= _board[move.y2()][move.x2()]->gamePhaseInfluence();
        _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[move.y2()][move.x2()]->getType(), _board[move.y2()][move.x2()]->isWhite(), move.x2(), move.y2());
        _evaluationValue -= _board[move.y2()][move.x2()]->evaluationValue(move.x2(), move.y2(), _gamePhase) * (_board[move.y2()][move.x2()]->isWhite() ? 1 : -1);

        _board[move.y2()][move.x2()] = promotionPiece;

        _gamePhase += _board[move.y2()][move.x2()]->gamePhaseInfluence();
        _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[move.y2()][move.x2()]->getType(), _board[move.y2()][move.x2()]->isWhite(), move.x2(), move.y2());
        _evaluationValue += _board[move.y2()][move.x2()]->evaluationValue(move.x2(), move.y2(), _gamePhase) * (_board[move.y2()][move.x2()]->isWhite() ? 1 : -1);
    }

    // Handle en passant move
    if (_board[move.y2()][move.x2()]->getType() == PieceType::Pawn) {
        if (move.y2() == 2 && upperEnPassantColumn() == move.x2()) {
            if (_board[3][move.x2()] != 0) {
                _gamePhase -= _board[3][move.x2()]->gamePhaseInfluence();
                _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[3][move.x2()]->getType(), _board[3][move.x2()]->isWhite(), move.x2(), 3);
                _evaluationValue -= _board[3][move.x2()]->evaluationValue(move.x2(), 3, _gamePhase) * (_board[3][move.x2()]->isWhite() ? 1 : -1);

                _board[3][move.x2()] = 0;
            }
        } else if (move.y2() == 5 && lowerEnPassantColumn() == move.x2()) {
            if (_board[4][move.x2()] != 0) {
                _gamePhase -= _board[4][move.x2()]->gamePhaseInfluence();
                _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[4][move.x2()]->getType(), _board[4][move.x2()]->isWhite(), move.x2(), 4);
                _evaluationValue -= _board[4][move.x2()]->evaluationValue(move.x2(), 4, _gamePhase) * (_board[4][move.x2()]->isWhite() ? 1 : -1);

                _board[4][move.x2()] = 0;
            }
        }
    }

    // Handle king move
    if (_board[move.y2()][move.x2()]->getType() == PieceType::King) {
		// Update king position
        if (_board[move.y2()][move.x2()]->isWhite()) {
			_whiteKingX = move.x2();
			_whiteKingY = move.y2();
        }
        else {
			_blackKingX = move.x2();
			_blackKingY = move.y2();
        }

        // Handle castling moves
        if (move.x2() == 2 && move.y2() == 0 && upperLeftCastlingPossible()) {
            if (_board[0][3] != 0) {
                _gamePhase -= _board[0][3]->gamePhaseInfluence();
                _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[0][3]->getType(), _board[0][3]->isWhite(), 3, 0);
                _evaluationValue -= _board[0][3]->evaluationValue(3, 0, _gamePhase) * (_board[0][3]->isWhite() ? 1 : -1);
            }

            _board[0][3] = _board[0][0];
            _board[0][0] = 0;

            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[0][3]->getType(), _board[0][3]->isWhite(), 0, 0);
            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[0][3]->getType(), _board[0][3]->isWhite(), 3, 0);
            _evaluationValue -= _board[0][3]->evaluationValue(0, 0, _gamePhase) * (_board[0][3]->isWhite() ? 1 : -1);
            _evaluationValue += _board[0][3]->evaluationValue(3, 0, _gamePhase) * (_board[0][3]->isWhite() ? 1 : -1);
        }
        else if (move.x2() == 6 && move.y2() == 0 && upperRightCastlingPossible()) {
            if (_board[0][5] != 0) {
                _gamePhase -= _board[0][5]->gamePhaseInfluence();
                _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[0][5]->getType(), _board[0][5]->isWhite(), 5, 0);
                _evaluationValue -= _board[0][5]->evaluationValue(5, 0, _gamePhase) * (_board[0][5]->isWhite() ? 1 : -1);
            }

            _board[0][5] = _board[0][7];
            _board[0][7] = 0;

            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[0][5]->getType(), _board[0][5]->isWhite(), 7, 0);
            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[0][5]->getType(), _board[0][5]->isWhite(), 5, 0);
            _evaluationValue -= _board[0][5]->evaluationValue(7, 0, _gamePhase) * (_board[0][5]->isWhite() ? 1 : -1);
            _evaluationValue += _board[0][5]->evaluationValue(5, 0, _gamePhase) * (_board[0][5]->isWhite() ? 1 : -1);
        }
        else if (move.x2() == 2 && move.y2() == 7 && lowerLeftCastlingPossible()) {
            if (_board[7][3] != 0) {
                _gamePhase -= _board[7][3]->gamePhaseInfluence();
                _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[7][3]->getType(), _board[7][3]->isWhite(), 3, 7);
                _evaluationValue -= _board[7][3]->evaluationValue(3, 7, _gamePhase) * (_board[7][3]->isWhite() ? 1 : -1);
            }

            _board[7][3] = _board[7][0];
            _board[7][0] = 0;

            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[7][3]->getType(), _board[7][3]->isWhite(), 0, 7);
            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[7][3]->getType(), _board[7][3]->isWhite(), 3, 7);
            _evaluationValue -= _board[7][3]->evaluationValue(0, 7, _gamePhase) * (_board[7][3]->isWhite() ? 1 : -1);
            _evaluationValue += _board[7][3]->evaluationValue(3, 7, _gamePhase) * (_board[7][3]->isWhite() ? 1 : -1);
        }
        else if (move.x2() == 6 && move.y2() == 7 && lowerRightCastlingPossible()) {
            if (_board[7][5] != 0) {
                _gamePhase -= _board[7][5]->gamePhaseInfluence();
                _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[7][5]->getType(), _board[7][5]->isWhite(), 5, 7);
                _evaluationValue -= _board[7][5]->evaluationValue(5, 7, _gamePhase) * (_board[7][5]->isWhite() ? 1 : -1);
            }

            _board[7][5] = _board[7][7];
            _board[7][7] = 0;
            
            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[7][5]->getType(), _board[7][5]->isWhite(), 7, 7);
            _hash = _hash xor GameInfo::getInstance()->pieceZobristValue(_board[7][5]->getType(), _board[7][5]->isWhite(), 5, 7);
            _evaluationValue -= _board[7][5]->evaluationValue(7, 7, _gamePhase) * (_board[7][5]->isWhite() ? 1 : -1);
            _evaluationValue += _board[7][5]->evaluationValue(5, 7, _gamePhase) * (_board[7][5]->isWhite() ? 1 : -1);
        }
    }

    // Update en passant flags
    if (_upperEnPassantColumn != -1) {
        _hash = _hash xor GameInfo::getInstance()->upperEnPassantZobristValue(_upperEnPassantColumn);
        _upperEnPassantColumn = -1;
    }

    if (_lowerEnPassantColumn != -1) {
        _hash = _hash xor GameInfo::getInstance()->lowerEnPassantZobristValue(_lowerEnPassantColumn);
        _lowerEnPassantColumn = -1;
    }

    if (_board[move.y2()][move.x2()]->getType() == PieceType::Pawn) {
        if (move.y2() == 3 && move.y2() - move.y1() == 2) {
            _upperEnPassantColumn = move.x2();
            _hash = _hash xor GameInfo::getInstance()->upperEnPassantZobristValue(_upperEnPassantColumn);
        }
        else if (move.y2() == 4 && move.y2() - move.y1() == -2) {
            _lowerEnPassantColumn = move.x2();
            _hash = _hash xor GameInfo::getInstance()->lowerEnPassantZobristValue(_lowerEnPassantColumn);
        }
    }

    // Update castling flags
    if (_upperLeftCastlingPossible && (((move.x1() == 4 && move.y1() == 0) || move.x1() == 0 && move.y1() == 0) || (move.x2() == 0 && move.y2() == 0))) {
        _upperLeftCastlingPossible = false;
        _hash = _hash xor GameInfo::getInstance()->upperLeftCastlingZobristValue();
    }
    if (_upperRightCastlingPossible && ((move.x1() == 4 && move.y1() == 0) || (move.x1() == 7 && move.y1() == 0) || (move.x2() == 7 && move.y2() == 0))) {
        _upperRightCastlingPossible = false;
        _hash = _hash xor GameInfo::getInstance()->upperRightCastlingZobristValue();
    }
    if (_lowerLeftCastlingPossible && ((move.x1() == 4 && move.y1() == 7) || (move.x1() == 0 && move.y1() == 7) || (move.x2() == 0 && move.y2() == 7))) {
        _lowerLeftCastlingPossible = false;
        _hash = _hash xor GameInfo::getInstance()->lowerLeftCastlingZobristValue();
    }
    if (_lowerRightCastlingPossible && ((move.x1() == 4 && move.y1() == 7) || (move.x1() == 7 && move.y1() == 7) || (move.x2() == 7 && move.y2() == 7))) {
        _lowerRightCastlingPossible = false;
        _hash = _hash xor GameInfo::getInstance()->lowerRightCastlingZobristValue();
    }
}

void GameState::applyNullMove() {
    // Update the last move
    _lastMove = Move(0, 0, 0, 0);

    // Change the side to move
    _isWhiteSideToMove = !_isWhiteSideToMove;
    _hash = _hash xor GameInfo::getInstance()->whiteSideToMoveZobristValue();
}

void GameState::printBoard() const {
    // Set the locale to support Unicode
    std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    std::wcout << L"    0   1   2   3   4   5   6   7" << std::endl;
    std::wcout << L"  +---+---+---+---+---+---+---+---+" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::wcout << 8 - i << L" |";
        for (int j = 0; j < 8; ++j) {
            wchar_t pieceChar = L' ';
            if (_board[i][j] != nullptr) {
                if (dynamic_cast<Queen*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265B' : L'\u2655';
                else if (dynamic_cast<Knight*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265E' : L'\u2658';
                else if (dynamic_cast<Bishop*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265D' : L'\u2657';
                else if (dynamic_cast<Rook*>(_board[i][j])) pieceChar = _board[i][j]->isWhite() ? L'\u265C' : L'\u2656';
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
        std::wcout << L" " << i << std::endl;
        std::wcout << L"  +---+---+---+---+---+---+---+---+" << std::endl;
    }
    std::wcout << L"    a   b   c   d   e   f   g   h" << std::endl;
}

Piece* GameState::getPieceAt(char x, char y) const {
    return _board[y][x];
}

void GameState::possibleNewGameStates(std::vector<GameState>& newGameStates, bool captureOnly) const {
    std::vector<Move> moves;
	moves.reserve(32);

    for (char i = 0; i < 8; i++) {
        for (char j = 0; j < 8; j++) {
            if (_board[i][j] == 0)
                continue;
            if (_board[i][j]->isWhite() != _isWhiteSideToMove)
                continue;

            _board[i][j]->possibleMoves(moves, j, i, *this, captureOnly);
        }
    }

    newGameStates.reserve(moves.size());
    for (char i = 0; i < moves.size(); i++) {
        newGameStates.emplace_back(*this);
        newGameStates.back().applyMove(moves[i]);

        if (newGameStates.back().isCheck(_isWhiteSideToMove)) {
            newGameStates.pop_back();
        }
    }
    
}

bool GameState::isCheck(bool isWhite) const {
	if (isWhite) {
        return isThreatened(isWhite, _whiteKingX, _whiteKingY);
	}

    return isThreatened(isWhite, _blackKingX, _blackKingY);
}

bool GameState::isThreatened(bool isWhite, char x, char y) const {
    // Check for rooks and queens at the right side of the square
    for (char i = x + 1; i < 8; i++) {
        if (_board[y][i] == 0)
            continue;
        if (_board[y][i]->isWhite() == isWhite)
            break;
		if (_board[y][i]->getType() != PieceType::Rook && _board[y][i]->getType() != PieceType::Queen)
			break;

        return true;
    }

    // Check for rooks and queens at the left side of the square
    for (char i = x - 1; i > -1; i--) {
        if (_board[y][i] == 0)
            continue;
        if (_board[y][i]->isWhite() == isWhite)
            break;
        if (_board[y][i]->getType() != PieceType::Rook && _board[y][i]->getType() != PieceType::Queen)
            break;

        return true;
    }

    // Check for rooks and queens at the down side of the square
    for (char i = y + 1; i < 8; i++) {
        if (_board[i][x] == 0)
            continue;
        if (_board[i][x]->isWhite() == isWhite)
            break;
        if (_board[i][x]->getType() != PieceType::Rook && _board[i][x]->getType() != PieceType::Queen)
            break;

        return true;
    }

    // Check for rooks and queens at the up side of the square
    for (char i = y - 1; i > -1; i--) {
        if (_board[i][x] == 0)
            continue;
        if (_board[i][x]->isWhite() == isWhite)
            break;
        if (_board[i][x]->getType() != PieceType::Rook && _board[i][x]->getType() != PieceType::Queen)
            break;

        return true;
    }

	// Check for bishops and queens at the lower right side of the square
    char j = y;
    for (char i = x + 1; i < 8; i++) {
        j++;
		if (j > 7)
			break;

        if (_board[j][i] == 0)
            continue;
        if (_board[j][i]->isWhite() == isWhite)
            break;
        if (_board[j][i]->getType() != PieceType::Bishop && _board[j][i]->getType() != PieceType::Queen)
            break;

        return true;
    }

    // Check for bishops and queens at the upper right side of the square
    j = y;
    for (char i = x + 1; i < 8; i++) {
        j--;
        if (j < 0)
            break;

        if (_board[j][i] == 0)
            continue;
        if (_board[j][i]->isWhite() == isWhite)
            break;
        if (_board[j][i]->getType() != PieceType::Bishop && _board[j][i]->getType() != PieceType::Queen)
            break;

        return true;
    }

    // Check for bishops and queens at the lower left side of the square
    j = y;
    for (char i = x - 1; i > -1; i--) {
        j++;
        if (j > 7)
            break;

        if (_board[j][i] == 0)
            continue;
        if (_board[j][i]->isWhite() == isWhite)
            break;
        if (_board[j][i]->getType() != PieceType::Bishop && _board[j][i]->getType() != PieceType::Queen)
            break;

        return true;
    }

    // Check for bishops and queens at the upper left side of the square
    j = y;
    for (char i = x - 1; i > -1; i--) {
        j--;
        if (j < 0)
            break;

        if (_board[j][i] == 0)
            continue;
        if (_board[j][i]->isWhite() == isWhite)
            break;
        if (_board[j][i]->getType() != PieceType::Bishop && _board[j][i]->getType() != PieceType::Queen)
            break;

        return true;
    }

	// Check for knights
	if (x - 2 > -1 && y - 1 > -1 && _board[y - 1][x - 2] != 0 && _board[y - 1][x - 2]->isWhite() != isWhite && _board[y - 1][x - 2]->getType() == PieceType::Knight) {
		return true;
	}
    if (x - 1 > -1 && y - 2 > -1 && _board[y - 2][x - 1] != 0 && _board[y - 2][x - 1]->isWhite() != isWhite && _board[y - 2][x - 1]->getType() == PieceType::Knight) {
        return true;
    }
    if (x + 1 < 8 && y - 2 > -1 && _board[y - 2][x + 1] != 0 && _board[y - 2][x + 1]->isWhite() != isWhite && _board[y - 2][x + 1]->getType() == PieceType::Knight) {
        return true;
    }
    if (x + 2 < 8 && y - 1 > -1 && _board[y - 1][x + 2] != 0 && _board[y - 1][x + 2]->isWhite() != isWhite && _board[y - 1][x + 2]->getType() == PieceType::Knight) {
        return true;
    }
    if (x + 2 < 8 && y + 1 < 8 && _board[y + 1][x + 2] != 0 && _board[y + 1][x + 2]->isWhite() != isWhite && _board[y + 1][x + 2]->getType() == PieceType::Knight) {
        return true;
    }
    if (x + 1 < 8 && y + 2 < 8 && _board[y + 2][x + 1] != 0 && _board[y + 2][x + 1]->isWhite() != isWhite && _board[y + 2][x + 1]->getType() == PieceType::Knight) {
        return true;
    }
    if (x - 1 > -1 && y + 2 < 8 && _board[y + 2][x - 1] != 0 && _board[y + 2][x - 1]->isWhite() != isWhite && _board[y + 2][x - 1]->getType() == PieceType::Knight) {
        return true;
    }
    if (x - 2 > -1 && y + 1 < 8 && _board[y + 1][x - 2] != 0 && _board[y + 1][x - 2]->isWhite() != isWhite && _board[y + 1][x - 2]->getType() == PieceType::Knight) {
        return true;
    }

	// Check for kings
    for (char i = x - 1; i <= x + 1; i++) {
		if (i < 0 || i > 7)
			continue;

        for (char j = y - 1; j <= y + 1; j++) {
            if (j < 0 || j > 7)
                continue;
			if (i == x && j == y)
				continue;

			if (_board[j][i] == 0)
				continue;
			if (_board[j][i]->isWhite() == isWhite)
				continue;
			if (_board[j][i]->getType() != PieceType::King)
				continue;

			return true;
        }
    }

	// Check for pawns
    if (isWhite) {
		if (x - 1 > -1 && y - 1 > -1 && _board[y - 1][x - 1] != 0 && _board[y - 1][x - 1]->isWhite() != isWhite && _board[y - 1][x - 1]->getType() == PieceType::Pawn) {
			return true;
		}
		if (x + 1 < 8 && y - 1 > -1 && _board[y - 1][x + 1] != 0 && _board[y - 1][x + 1]->isWhite() != isWhite && _board[y - 1][x + 1]->getType() == PieceType::Pawn) {
			return true;
		}
	}
	else {
		if (x - 1 > -1 && y + 1 < 8 && _board[y + 1][x - 1] != 0 && _board[y + 1][x - 1]->isWhite() != isWhite && _board[y + 1][x - 1]->getType() == PieceType::Pawn) {
			return true;
		}
		if (x + 1 < 8 && y + 1 < 8 && _board[y + 1][x + 1] != 0 && _board[y + 1][x + 1]->isWhite() != isWhite && _board[y + 1][x + 1]->getType() == PieceType::Pawn) {
			return true;
		}
    }

    return false;
}

int GameState::evaluationValue(bool isWhite) const {
    return isWhite ? _evaluationValue : -_evaluationValue;
}

bool GameState::isWhiteSideToMove() const {
    return _isWhiteSideToMove;
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

char GameState::upperEnPassantColumn() const {
    return _upperEnPassantColumn;
}

char GameState::lowerEnPassantColumn() const {
    return _lowerEnPassantColumn;
}

Move GameState::lastMove() const {
    return _lastMove;
}

char GameState::gamePhase() const {
    return _gamePhase;
}

uint64_t GameState::hash() const {
    return _hash;
}
