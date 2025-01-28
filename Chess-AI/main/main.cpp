#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "gameState/gameState.h"
#include "gameState/currentGameState.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "move.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);
	CurrentGameState gameState;
	gameState.printBoard();

	while (true) {

		// Piece tests go after this line <----------------------------------------------
		
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Queen* queenTest = dynamic_cast<Queen*>(gameState.getPieceAt(i, j));
                if (queenTest && queenTest->isWhite()) {
                    std::vector<Move> moves;
                    queenTest->possibleMoves(moves, i, j, gameState);
                    if (moves.empty()) {
                        std::cout << "No moves available for white queen at (" << i << "," << j << ").\n";
                    }
                    else {
                        for (const auto& move : moves) {
                            std::cout << "White queen at (" << i << "," << j << ") can move to ("
                                << move.x2() << "," << move.y2() << ")\n";
                        }
                    }
                }
            }
        }

        // Collect and print moves for all white bishops
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Bishop* bishopTest = dynamic_cast<Bishop*>(gameState.getPieceAt(i, j));
				// Makes sure that the piece is a bishop and not a queen (since queen inherits bishop)
                Bishop* queenTest = dynamic_cast<Queen*>(gameState.getPieceAt(i, j));
                if (queenTest == 0 && bishopTest && bishopTest->isWhite()) {
                    std::vector<Move> moves;
                    bishopTest->possibleMoves(moves, i, j, gameState);
                    if (moves.empty()) {
                        std::cout << "No moves available for white bishop at (" << i << "," << j << ").\n";
                    }
                    else {
                        for (const auto& move : moves) {
                            std::cout << "White bishop at (" << i << "," << j << ") can move to ("
                                << move.x2() << "," << move.y2() << ")\n";
                        }
                    }
                }
            }
        }

        // Collect and print moves for all white rooks
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Rook* rookTest = dynamic_cast<Rook*>(gameState.getPieceAt(i, j));
				// Makes sure that the piece is a rook and not a queen (since queen inherits rook)
                Rook* queenTest = dynamic_cast<Queen*>(gameState.getPieceAt(i, j));
                if (queenTest == 0 && rookTest && rookTest->isWhite()) {
                    std::vector<Move> moves;
                    rookTest->possibleMoves(moves, i, j, gameState);
                    if (moves.empty()) {
                        std::cout << "No moves available for white rook at (" << i << "," << j << ").\n";
                    }
                    else {
                        for (const auto& move : moves) {
                            std::cout << "White rook at (" << i << "," << j << ") can move to ("
                                << move.x2() << "," << move.y2() << ")\n";
                        }
                    }
                }
            }
        }

        // Collect and print moves for all white knights
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Knight* knightTest = dynamic_cast<Knight*>(gameState.getPieceAt(i, j));
                if (knightTest && knightTest->isWhite()) {
                    std::vector<Move> moves;
                    knightTest->possibleMoves(moves, i, j, gameState);
                    if (moves.empty()) {
                        std::cout << "No moves available for white knight at (" << i << "," << j << ").\n";
                    }
                    else {
                        for (const auto& move : moves) {
                            std::cout << "White knight at (" << i << "," << j << ") can move to ("
                                << move.x2() << "," << move.y2() << ")\n";
                        }
                    }
                }
            }
        }

		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				Pawn* pawnTest = dynamic_cast<Pawn*>(gameState.getPieceAt(i, j));
				if (pawnTest && pawnTest->isWhite()) {
                    std::vector<Move> moves;
					pawnTest->possibleMoves(moves, i, j, gameState);
					if (moves.empty()) {
						std::cout << "No moves available for white pawn at (" << i << "," << j << ").\n";
					}
					else {
						for (const auto& move : moves) {
							std::cout << "White pawn at (" << i << "," << j << ") can move to ("
								<< move.x2() << "," << move.y2() << ")\n";
						}
					}
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

    // Collect and print moves for all white queens
    




	return 0;
}
