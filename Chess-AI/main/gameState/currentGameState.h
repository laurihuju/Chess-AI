#ifndef CURRENTGAMESTATE_H
#define CURRENTGAMESTATE_H

#include "gameState.h"

class CurrentGameState : public GameState {

private:
	static CurrentGameState* instance;

public:
	/// <summary>
	/// Initializes the GameState with default board content.
	/// </summary>
	CurrentGameState();
	CurrentGameState(const CurrentGameState&) = delete;

	static CurrentGameState* getInstance();

};

#endif
