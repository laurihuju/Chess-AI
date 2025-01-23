#ifndef GAMESTATE_H
#define GAMESTATE_H

class Piece;
class King;

class GameState {

private:
	Piece* _board[8][8];

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

	void setUpperLeftCastlingNotPossible();
	void setUpperRightCastlingNotPossible();
	void setLowerLeftCastlingNotPossible();
	void setLowerRightCastlingNotPossible();

};

#endif
