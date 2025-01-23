#include <windows.h>
#include <iostream>
#include "gameState.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "move.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);
    GameState gameState;
    gameState.printBoard();
    // Bishop* bishopTest = dynamic_cast<Bishop*>(gameState.getPieceAt(4,4));
    // if(bishopTest) {
    //     auto moves = bishopTest->possibleMoves(4,4, gameState);
    //     if (moves.empty()) {
    //         std::cout << "No moves available for this bishop.\n";
    //     } else {
    //         for (const auto& move : moves) {
    //             std::cout << "Bishop can move from (" << move.x1() << "," << move.y1()
    //                       << ") to (" << move.x2() << "," << move.y2() << ")\n";
    //         }
    //     }
    // }

    // Rook* rookTest = dynamic_cast<Rook*>(gameState.getPieceAt(0,7));
    // if(rookTest) {
    //     auto moves = rookTest->possibleMoves(0,7, gameState);
    //     if (moves.empty()) {
    //         std::cout << "No moves available for this rook.\n";
    //     } else {
    //         for (const auto& move : moves) {
    //             std::cout << "Rook can move from (" << move.x1() << "," << move.y1()
    //                       << ") to (" << move.x2() << "," << move.y2() << ")\n";
    //         }
    //     }
    // }

    Queen* queenTest = dynamic_cast<Queen*>(gameState.getPieceAt(3,7));
    if (queenTest) {
        auto moves = queenTest->possibleMoves(3,7, gameState);
        if (moves.empty()) {
            std::cout << "No moves available for this queen.\n";
        } else {
            for (const auto& move : moves) {
                std::cout << "Queen can move from (" << move.x1() << "," << move.y1()
                          << ") to (" << move.x2() << "," << move.y2() << ")\n";
            }
        }
    }
    return 0;
}
