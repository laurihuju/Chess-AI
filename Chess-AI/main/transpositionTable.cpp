#include "transpositionTable.h"

template<size_t Size>
inline void TranspositionTable<Size>::store(const GameState& state, int evaluationValue, int evaluationDepth, const Move& bestMove, TranspositionTableItemType itemType) {
	// Calculate the transposition table slot of the game state
	int index = state.hash() % Size;

	// Don't update value if the slot has a deeper value than the new value
	if (_items[index].evaluationDepth > evaluationDepth) {
		return;
	}

	// Update the new values to the slot
	_items[index].itemType = itemType;
	_items[index].hash = state.hash();

	_items[index].evaluationValue = evaluationValue;
	_items[index].evaluationDepth = evaluationDepth;
	_items[index].bestMove = bestMove;
}

template<size_t Size>
bool TranspositionTable<Size>::lookup(const GameState& state, int minDepth, int& evaluationValue, Move& bestMove, TranspositionTableItemType& itemType) {
	// Calculate the transposition table slot of the game state
	int index = state.hash() % Size;

	// Correct result was not found if the slot doesn't contain item with the correct hash or the item has too small depth
	if (_items[index].hash != state.hash() || _items[index].evaluationDepth < minDepth) {
		return false;
	}

	// Set the evaluation value, best value, and item type reference parameters to the values of the table item
	evaluationValue = _items[index].evaluationValue;
	bestMove = _items[index].bestMove;
	itemType = _items[index].itemType;

	// Return true as correct result was found
	return true;
}

template class TranspositionTable<30000000>;
