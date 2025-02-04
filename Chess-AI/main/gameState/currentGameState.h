#ifndef CURRENTGAMESTATE_H
#define CURRENTGAMESTATE_H

#include "gameState.h"

/// <summary>
/// Describes the current game state (not any other possible game state). Create only one these as a time.
/// </summary>
class CurrentGameState : public GameState {

private:
	/// <summary>
	/// The singleton instance of CurrentGameState.
	/// </summary>
	static CurrentGameState* instance;

public:
	/// <summary>
	/// Initializes the GameState with normal chess starting board content.
	/// </summary>
	CurrentGameState();
	CurrentGameState(const CurrentGameState&) = delete;

	/// <summary>
	/// The singleton instance of CurrentGameState.
	/// </summary>
	/// <returns>The singleton instance of CurrentGameState.</returns>
	static CurrentGameState* getInstance();

};

#endif
