#include <windows.h>
#include <locale>
#include <codecvt>
#include <iostream>
#include "gameState.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);
    GameState gameState;
    gameState.printBoard();
    return 0;
}
