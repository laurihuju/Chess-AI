#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <mutex>
#include "move.h"
#include "gameState/gameState.h"

/// <summary>
/// An enum class describing transposition table item type.
/// </summary>
enum class TranspositionTableItemType {
	/// <summary>
	/// The transposition table item has exact minimax evaluation value.
	/// </summary>
	Exact,

	/// <summary>
	/// The exact minimax evaluation value of the transposition table item is not known due to pruning
	/// and the item's evaluation value is at least the stored value. These values have to be used
	/// differently than exact values.
	/// </summary>
	LowerBound,

	/// <summary>
	/// The exact minimax evaluation value of the transposition table item is not known due to pruning
	/// and the item's evaluation value is at most the stored value. These values have to be used
	/// differently than exact values.
	/// </summary>
	UpperBound
};

/// <summary>
/// A struct describing transposition table item.
/// </summary>
struct TranspositionTableItem {
	/// <summary>
	/// The type of the item.
	/// </summary>
	TranspositionTableItemType itemType = TranspositionTableItemType::Exact;

	/// <summary>
	/// The hash of the item.
	/// </summary>
	uint64_t hash = 0;

	/// <summary>
	/// The evaluation value of the item.
	/// </summary>
	int evaluationValue = 0;

	/// <summary>
	/// The evaluation depth of the item.
	/// </summary>
	int evaluationDepth = 0;

	/// <summary>
	/// If the item was evaluated for white.
	/// </summary>
	bool evaluatedForWhite = true;

	/// <summary>
	/// The best move of the item.
	/// </summary>
	Move bestMove = Move(0, 0, 0, 0);

};

/// <summary>
/// A template class describing a transposition table of the given size.
/// </summary>
/// <typeparam name="Size">The amount of items that the table can store</typeparam>
template <size_t Size>
class TranspositionTable {

private:
	/// <summary>
	/// The transposition table items.
	/// </summary>
	std::atomic<TranspositionTableItem> _items[Size];
	
public:
	/// <summary>
	/// Handles storing the given game state with the given evaluation value, depth and item type.
	/// Handles collisions in the table by replacing the older item.
	/// Doesn't replace any items with higher depth than the given depth.
	/// The function is thread safe as it uses atomic operations.
	/// </summary>
	/// <param name="state">The game state to store</param>
	/// <param name="evaluationValue">The evaluation value</param>
	/// <param name="evaluationDepth">The evaluation depth</param>
	/// <param name="evaluatedForWhite">If the game state was evaluated for white</param>
	/// <param name="itemType">The item type</param>
	/// <param name="bestMove">The best move</param>
	void store(const GameState& state, int evaluationValue, int evaluationDepth, bool evaluatedForWhite, TranspositionTableItemType itemType, const Move& bestMove);

	/// <summary>
	/// Performs a table lookup with the given game state and minimum depth.
	/// If an item with at least the minimum depth and the same hash with the given game state is found,
	/// sets the evaluation value and item type of the item to the reference parameter variables.
	/// The function is thread safe as it uses atomic operations.
	/// </summary>
	/// <param name="state">The game state to lookup</param>
	/// <param name="minDepth">The minumum depth to lookup</param>
	/// <param name="evaluateForWhite">If the lookup should search result for white (or black)</param>
	/// <param name="evaluationValue">Reference parameter that gets the evaluation value if an item is found</param>
	/// <param name="bestMove">Reference parameter that gets the best move if an item is found</param>
	/// <param name="itemType">Reference parameter that gets the item type if an item is found</param>
	/// <returns>True if an item was found and the reference parameters were updated</returns>
	bool lookup(const GameState& state, int minDepth, bool evaluateForWhite, int& evaluationValue, Move& bestMove, TranspositionTableItemType& itemType);

};

#endif
