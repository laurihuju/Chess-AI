#ifndef CURRENTGAMESTATE_H
#define CURRENTGAMESTATE_H

#include "../piece.h"
#include "gameState.h"

/// <summary>
/// Describes the current game state (not any other possible game state). Create only one these at a time.
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

	/// <summary>
	/// Deleted copy constructor.
	/// </summary>
	/// <param name=""></param>
	CurrentGameState(const CurrentGameState&) = delete;

	/// <summary>
	/// The singleton instance of CurrentGameState.
	/// </summary>
	/// <returns>The singleton instance of CurrentGameState.</returns>
	static CurrentGameState* getInstance();

};

#endif
