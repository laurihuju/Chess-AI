#ifndef move_h
#define move_h

#include "include.h"

class Move {

private:
	int _x1;
	int _y1;

	int _x2;
	int _y2;

	int convertXCoordinateFromInput(char coordinateInputChar) const;
	int convertYCoordinateFromInput(char coordinateInputChar) const;

public:
	Move(int x1, int y1, int x2, int y2);
	Move(const std::string& input);
	Move();

	int x1() const;
	int y1() const;

	int x2() const;
	int y2() const;

};


#endif
