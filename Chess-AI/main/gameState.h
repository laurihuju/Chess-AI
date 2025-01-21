#pragma once

#include "piece.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/queen.h"
#include "pieces/rook.h"

class GameState {

private:
	Piece* _board[8][8] = {
		{new Rook(false), new Knight(false), new Bishop(false), new Queen(false), new King(false), new Bishop(false), new Knight(false), new Rook(false)},
		{new Pawn(false), new Pawn(false), new Pawn(false), new Pawn(false), new Pawn(false), new Pawn(false), new Pawn(false), new Pawn(false)},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{new Pawn(true), new Pawn(true), new Pawn(true), new Pawn(true), new Pawn(true), new Pawn(true), new Pawn(true), new Pawn(true)},
		{new Rook(true), new Knight(true), new Bishop(true), new Queen(true), new King(true), new Bishop(true), new Knight(true), new Rook(true)}
	};

	// Castling
	bool _upperLeftCastlingPossible = true;
	bool _upperRightCastlingPossible = true;
	bool _lowerLeftCastlingPossible = true;
	bool _lowerRightCastlingPossible = true;

public:
	GameState();
	~GameState();

	King* findKing(bool isWhite, int& x, int& y) const;
  void printBoard() const;
  
	// Castling
	bool upperLeftCastlingPossible() const;
	bool upperRightCastlingPossible() const;
	bool lowerLeftCastlingPossible() const;
	bool lowerRightCastlingPossible() const;

	bool setUpperLeftCastlingNotPossible();
	bool setUpperRightCastlingNotPossible();
	bool setLowerLeftCastlingNotPossible();
	bool setLowerRightCastlingNotPossible();
  
};