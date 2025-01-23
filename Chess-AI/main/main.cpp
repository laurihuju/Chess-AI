#include <windows.h>
#include "gameState.h"

int main() {
	  SetConsoleOutputCP(CP_UTF8);
    GameState gameState;
    gameState.printBoard();
    return 0;
}
