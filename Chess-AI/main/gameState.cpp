#include "gameState.h"

GameState::GameState() {

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
