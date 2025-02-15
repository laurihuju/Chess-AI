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

	/// <summary>
	/// All pieces created by the current game state.
	/// These are deleted when the current game state gets deleted.
	/// </summary>
	std::vector<Piece*> _pieces;

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
	/// Destroys the current game state and all pieces it has created.
	/// </summary>
	virtual ~CurrentGameState();

	/// <summary>
	/// Searches the given type and color piece from the piece instances
	/// created by this current game state.
	/// </summary>
	/// <param name="type">The piece type to search</param>
	/// <param name="isWhite">If to search white piece</param>
	/// <returns>The found piece, or nullptr if not found</returns>
	Piece* getPieceInstance(PieceType type, bool isWhite) const;

	/// <summary>
	/// The singleton instance of CurrentGameState.
	/// </summary>
	/// <returns>The singleton instance of CurrentGameState.</returns>
	static CurrentGameState* getInstance();

};

#endif
