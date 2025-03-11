#include "transpositionTable.h"

template<size_t Size>
inline void TranspositionTable<Size>::store(const GameState& state, int evaluationValue, int evaluationDepth, bool evaluatedForWhite, TranspositionTableItemType itemType, const Move& bestMove) {
	// Calculate the transposition table slot of the game state
	int index = state.hash() % Size;

	// Don't update value if the slot has a deeper value than the new value
	TranspositionTableItem item = _items[index].load();
	if (item.evaluationDepth > evaluationDepth) {
		return;
	}

	// Update the new values to the slot
	item.itemType = itemType;
	item.hash = state.hash();

	item.evaluationValue = evaluationValue;
	item.evaluationDepth = evaluationDepth;
	item.evaluatedForWhite = evaluatedForWhite;
	item.bestMove = bestMove;

	_items[index].store(item);
}

template<size_t Size>
bool TranspositionTable<Size>::lookup(const GameState& state, int minDepth, bool evaluateForWhite, int& evaluationValue, Move& bestMove, TranspositionTableItemType& itemType) {
	// Calculate the transposition table slot of the game state
	int index = state.hash() % Size;

	// Correct result was not found if the slot doesn't contain item with the correct hash or the item has too small depth
	TranspositionTableItem item = _items[index].load();
	if (item.hash != state.hash() || item.evaluatedForWhite != evaluateForWhite || item.evaluationDepth < minDepth) {
		return false;
	}

	// Set the evaluation value, best value, and item type reference parameters to the values of the table item
	evaluationValue = item.evaluationValue;
	bestMove = item.bestMove;
	itemType = item.itemType;

	// Return true as correct result was found
	return true;
}

template class TranspositionTable<30000000>;
