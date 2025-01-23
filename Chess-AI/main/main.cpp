#include <windows.h>
#include <iostream>
#include <string>
#include "gameState.h"
#include "pieces/bishop.h"
#include "move.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);
    GameState gameState;
    gameState.printBoard();

    while (true) {
        std::string input;
        std::cin >> input;

        Move move(input);
        gameState.applyMove(move);
        gameState.printBoard();
    }

    //Bishop* bishopTest = dynamic_cast<Bishop*>(gameState.getPieceAt(4,4));
    //if(bishopTest) {
    //    auto moves = bishopTest->possibleMoves(4,4, gameState);
    //    if (moves.empty()) {
    //        std::cout << "No moves available for this bishop.\n";
    //    } else {
    //        for (const auto& move : moves) {
    //            std::cout << "Bishop can move from (" << move.x1() << "," << move.y1()
    //                      << ") to (" << move.x2() << "," << move.y2() << ")\n";
    //        }
    //    }
    //}

    return 0;
}
