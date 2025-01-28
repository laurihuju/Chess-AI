#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "gameState.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/king.h"
#include "move.h"

// Helper function to convert coordinates to chess notation
std::string indexToCoord(int x, int y) {
	return std::string(1, char('a' + x)) + std::string(1, char('8' - y));
}

// Helper function to set console text color
void setConsoleColor(WORD color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	GameState gameState;
	gameState.printBoard();

	while (true) {
		// Piece tests go after this line <----------------------------------------------

		// Test for kings
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				King* kingTest = dynamic_cast<King*>(gameState.getPieceAt(i, j));
				if (kingTest && kingTest->isWhite()) {
					std::vector<Move> moves;
					kingTest->possibleMoves(moves, i, j, gameState);
					setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // Set color for king
					std::cout << "wK at: " << indexToCoord(i, j) << "; moves:";
					setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
					if (moves.empty()) {
						std::cout << " N/A";
					}
					else {
						for (const auto& move : moves) {
							std::cout << " " << indexToCoord(move.x2(), move.y2());
						}
					}
					std::cout << "\n";
				}
			}
		}

		// Test for queens
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				Queen* queenTest = dynamic_cast<Queen*>(gameState.getPieceAt(i, j));
				if (queenTest && queenTest->isWhite()) {
					std::vector<Move> moves;
					queenTest->possibleMoves(moves, i, j, gameState);
					setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Set color for queen
					std::cout << "wQ at: " << indexToCoord(i, j) << "; moves:";
					setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
					if (moves.empty()) {
						std::cout << " N/A";
					}
					else {
						for (const auto& move : moves) {
							std::cout << " " << indexToCoord(move.x2(), move.y2());
						}
					}
					std::cout << "\n";
				}
			}
		}

		// Test for bishops
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				Bishop* bishopTest = dynamic_cast<Bishop*>(gameState.getPieceAt(i, j));
				Bishop* queenTest = dynamic_cast<Queen*>(gameState.getPieceAt(i, j));
				if (queenTest == 0 && bishopTest && bishopTest->isWhite()) {
					std::vector<Move> moves;
					bishopTest->possibleMoves(moves, i, j, gameState);
					setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Set color for bishop
					std::cout << "wB at: " << indexToCoord(i, j) << "; moves:";
					setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
					if (moves.empty()) {
						std::cout << " N/A";
					}
					else {
						for (const auto& move : moves) {
							std::cout << " " << indexToCoord(move.x2(), move.y2());
						}
					}
					std::cout << "\n";
				}
			}
		}

		// Test for rooks
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				Rook* rookTest = dynamic_cast<Rook*>(gameState.getPieceAt(i, j));
				Rook* queenTest = dynamic_cast<Queen*>(gameState.getPieceAt(i, j));
				if (queenTest == 0 && rookTest && rookTest->isWhite()) {
					std::vector<Move> moves;
					rookTest->possibleMoves(moves, i, j, gameState);
					setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Set color for rook
					std::cout << "wR at: " << indexToCoord(i, j) << "; moves:";
					setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
					if (moves.empty()) {
						std::cout << " N/A";
					}
					else {
						for (const auto& move : moves) {
							std::cout << " " << indexToCoord(move.x2(), move.y2());
						}
					}
					std::cout << "\n";
				}
			}
		}

		// Test for knights
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				Knight* knightTest = dynamic_cast<Knight*>(gameState.getPieceAt(i, j));
				if (knightTest && knightTest->isWhite()) {
					std::vector<Move> moves;
					knightTest->possibleMoves(moves, i, j, gameState);
					setConsoleColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Set color for knight
					std::cout << "wN at: " << indexToCoord(i, j) << "; moves:";
					setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
					if (moves.empty()) {
						std::cout << " N/A";
					}
					else {
						for (const auto& move : moves) {
							std::cout << " " << indexToCoord(move.x2(), move.y2());
						}
					}
					std::cout << "\n";
				}
			}
		}

		// Test for pawns
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				Pawn* pawnTest = dynamic_cast<Pawn*>(gameState.getPieceAt(i, j));
				if (pawnTest && pawnTest->isWhite()) {
					std::vector<Move> moves;
					pawnTest->possibleMoves(moves, i, j, gameState);
					setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Set color for pawn
					std::cout << "wP at: " << indexToCoord(i, j) << "; moves:";
					setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
					if (moves.empty()) {
						std::cout << " N/A";
					}
					else {
						for (const auto& move : moves) {
							std::cout << " " << indexToCoord(move.x2(), move.y2());
						}
					}
					std::cout << "\n";
				}
			}
		}

		// Piece tests go before this line <----------------------------------------------

		std::string input;
		std::cin >> input;

		Move move(input);
		gameState.applyMove(move);
		gameState.printBoard();
	}

	return 0;
}



