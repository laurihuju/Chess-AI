#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <iostream>
#include <stack>

#include "raylib.h"

#include "gameState/gameState.h"
#include "gameState/gameInfo.h"
#include "move.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/queen.h"
#include "pieces/rook.h"
#include "chessAI.h"

/// <summary>
/// Loads piece textures and adds them to the given unordered map.
/// Sets the values of the following keys:
/// PW (white pawn), PB (black pawn), RW (white rook), RB (black rook), NW (white knight), NB (black knight),
/// BW (white bishop), BB (black bishop), QW (white queen), QB (black queen), KW (white king), KB (black king)
/// </summary>
/// <param name="textures"></param>
void loadPieceTextures(std::unordered_map<std::string, Texture2D>& textures);

/// <summary>
/// Checks input in the UI and handles updating the game state, selected square and possible moves.
/// </summary>
/// <param name="gameState">The current game state</param>
/// <param name="selectedSquare">The selected square coordinates</param>
/// <param name="possibleMoves">The vector of possible moves</param>
/// <param name="lastMove">The last move</param>
/// <param name="previousStates">The stack of previous game states</param>
/// <param name="nextStates">The stack of next game states</param>
/// <param name="boardSize">The board width and height</param>
/// <param name="boardOffsetX">The board X offset from the window 0 coordinate</param>
/// <param name="boardOffsetY">The board Y offset from the window 0 coordinate</param>
void handleInput(GameState& gameState, Vector2& selectedSquare, std::vector<Move>& possibleMoves, Move& lastMove, 
    std::stack<GameState>& previousStates, std::stack<GameState>& nextStates, int boardSize, int boardOffsetX, int boardOffsetY, bool& isFlipped);

/// <summary>
/// Draws the board content to the window.
/// </summary>
/// <param name="gameState">The current game state</param>
/// <param name="possibleMoves">The vector of possible moves</param>
/// <param name="selectedSquare">The selected square coordinates</param
/// <param name="lastMove">The last move</param>
/// <param name="textures">Textures loaded with loadPieceTextures()</param>
/// <param name="boardSize">The board width and height</param>
/// <param name="boardOffsetX">The board X offset from the window 0 coordinate</param>
/// <param name="boardOffsetY">The board Y offset from the window 0 coordinate</param>
/// <param name="isFlipped">Whether the board is in flipped orientation</param>
void drawBoard(const GameState& gameState, const std::vector<Move>& possibleMoves, const Vector2& selectedSquare, const Move& lastMove,
    const std::unordered_map<std::string, Texture2D>& textures, int boardSize, int boardOffsetX, int boardOffsetY, bool isFlipped);

/// <summary>
/// Draws the given piece at the given coordinates to the window.
/// </summary>
/// <param name="piece">The piece to draw</param>
/// <param name="x">The X coordinate of the piece</param>
/// <param name="y">The Y coordinate of the piece</param>
/// <param name="textures">Textures loaded with loadPieceTextures()</param>
/// <param name="squareSize">Size of one square of the board</param>
/// <param name="offsetX">The board X offset from the window 0 coordinate</param>
/// <param name="offsetY">The board Y offset from the window 0 coordinate</param>
/// <param name="isFlipped">Whether the board is in flipped orientation</param>
void drawPiece(Piece* piece, int x, int y, const std::unordered_map<std::string, Texture2D>& textures, int squareSize, int offsetX, int offsetY, bool isFlipped);

void startGameUi()
{
    // Initialize the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1080, 1080, "Chess Game");
    SetTargetFPS(60);
    MaximizeWindow();

    // The game info and current game state
    GameInfo gameInfo;
    GameState gameState;

    // The selected square, possible moves and the last move
    Vector2 selectedSquare = { -1, -1 };
    std::vector<Move> possibleMoves;
	Move lastMove = Move(0, 0, 0, 0);

    // The stacks of previous and next game states stack for the undo and redo features
	std::stack<GameState> previousStates;
	std::stack<GameState> nextStates;

    // Track if the board is flipped (false = white at bottom, true = black at bottom)
    bool isFlipped = false;

    // Load textures once and store them in a map
    std::unordered_map<std::string, Texture2D> textures;
    loadPieceTextures(textures);

    while (!WindowShouldClose())
    {
        // Get current window size
        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();

        // Calculate board size and offset
        int boardSize = std::min(currentScreenWidth, currentScreenHeight);
        int boardOffsetX = (currentScreenWidth - boardSize) / 2;
        int boardOffsetY = (currentScreenHeight - boardSize) / 2;

        // Read input from the user
        handleInput(gameState, selectedSquare, possibleMoves, lastMove, previousStates, nextStates, boardSize, boardOffsetX, boardOffsetY, isFlipped);
        
        // Render the board
        drawBoard(gameState, possibleMoves, selectedSquare, lastMove, textures, boardSize, boardOffsetX, boardOffsetY, isFlipped);
    }

    // Unload textures
    for (auto& texture : textures)
    {
        UnloadTexture(texture.second);
    }

    CloseWindow();
    return;
}

void loadPieceTextures(std::unordered_map<std::string, Texture2D>& textures) {
    textures["PW"] = LoadTexture("main/resources/white_pawn.png");
    textures["PB"] = LoadTexture("main/resources/black_pawn.png");
    textures["RW"] = LoadTexture("main/resources/white_rook.png");
    textures["RB"] = LoadTexture("main/resources/black_rook.png");
    textures["NW"] = LoadTexture("main/resources/white_knight.png");
    textures["NB"] = LoadTexture("main/resources/black_knight.png");
    textures["BW"] = LoadTexture("main/resources/white_bishop.png");
    textures["BB"] = LoadTexture("main/resources/black_bishop.png");
    textures["QW"] = LoadTexture("main/resources/white_queen.png");
    textures["QB"] = LoadTexture("main/resources/black_queen.png");
    textures["KW"] = LoadTexture("main/resources/white_king.png");
    textures["KB"] = LoadTexture("main/resources/black_king.png");
}

void handleInput(GameState& gameState, Vector2& selectedSquare, std::vector<Move>& possibleMoves, Move& lastMove, 
    std::stack<GameState>& previousStates, std::stack<GameState>& nextStates, int boardSize, int boardOffsetX, int boardOffsetY, bool& isFlipped) {
    // Flip the board when pressing F key
    if (IsKeyPressed(KEY_F)) {
        isFlipped = !isFlipped;
        std::cout << "Board flipped. " << (isFlipped ? "Black" : "White") << " side is now at the bottom." << std::endl;
        
        // Clear selection when flipping
        selectedSquare = { -1, -1 };
        possibleMoves.clear();
        return;
    }
    
    // Use AI to complete the move when pressing space
    if (IsKeyPressed(KEY_SPACE)) {
        // Find the best move and calculate the calculation time
        auto startTime = std::chrono::high_resolution_clock::now();
        Move aiMove = ChessAI::findBestMove(gameState, 20); // Max depth 20 with 4 second time limit
        auto endTime = std::chrono::high_resolution_clock::now();

        // Print debugging data
        std::cout << "Calculation time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms\n";
        std::cout << (gameState.isWhiteSideToMove() ? "White" : "Black") << ": (" << (int)aiMove.x1() << "; " << (int)aiMove.y1() << ") -> (" << (int)aiMove.x2() << "; " << (int)aiMove.y2() << ")" << "\n";

        // Remove selection
        selectedSquare = { -1, -1 };
        possibleMoves.clear();

        // If AI returned an empty move, there are no moves available
        if (aiMove == Move(0, 0, 0, 0)) {
            return;
        }

		GameState previousState(gameState);
		previousStates.push(previousState);
		if (!nextStates.empty()) {
			nextStates = std::stack<GameState>();
		}

        gameState.applyMove(aiMove);
		lastMove = aiMove;
        return;
    }
    // Undo move
    if (IsKeyPressed(KEY_LEFT)) {
        if (previousStates.size() == 0) {
            return;
        }

		GameState nextState(gameState);
		nextStates.push(nextState);

		gameState = previousStates.top();
		previousStates.pop();
		lastMove = gameState.lastMove();

		selectedSquare = { -1, -1 };
		possibleMoves.clear();
    }

	// Redo move
    if (IsKeyPressed(KEY_RIGHT)) {
        if (nextStates.size() == 0) {
            return;
        }

		GameState previousState(gameState);
		previousStates.push(previousState);

		gameState = nextStates.top();
		nextStates.pop();
		lastMove = gameState.lastMove();

		selectedSquare = { -1, -1 };
		possibleMoves.clear();
    }


    // Handle mouse left click input
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return;
    }

    // Get the current mouse input and remove board offsets from it
    Vector2 mousePos = GetMousePosition();
    mousePos.x -= boardOffsetX;
    mousePos.y -= boardOffsetY;

    // Click outside board (remove selection)
    if (mousePos.x < 0 || mousePos.x > boardSize || mousePos.y < 0 || mousePos.y > boardSize) {
        selectedSquare = { -1, -1 };
        possibleMoves.clear();

        return;
    }

    // Calculate the file and rank of the clicked mouse position
    int file = mousePos.x / (boardSize / 8);
    int rank = mousePos.y / (boardSize / 8);
    
    // Adjust coordinates if the board is flipped
    if (isFlipped) {
        file = 7 - file;
        rank = 7 - rank;
    }

    // First click (select piece)
    if (selectedSquare.x == -1)
    {
        Piece* selectedPiece = gameState.getPieceAt(file, rank);
        if (selectedPiece)
        {
            selectedSquare = { (float)file, (float)rank };

            // There are no possible moves if moving piece at wrong turn
            if (selectedPiece->isWhite() == gameState.isWhiteSideToMove()) {
                selectedPiece->possibleMoves(possibleMoves, file, rank, gameState);

                // Do not show moves that would lead the player to check
                for (int i = 0; i < possibleMoves.size(); i++) {
                    GameState testState(gameState);
                    testState.applyMove(possibleMoves[i]);

                    if (testState.isCheck(gameState.isWhiteSideToMove())) {
                        possibleMoves.erase(possibleMoves.begin() + i);
                        i--;
                    }
                }
            }
        }

        return;
    }

    // Second click (move the selected piece if possible)
    Piece* movingPiece = gameState.getPieceAt(selectedSquare.x, selectedSquare.y);
    if (movingPiece != 0 && movingPiece->isWhite() != gameState.isWhiteSideToMove()) {
        selectedSquare = { -1, -1 };
        possibleMoves.clear();

        return;
    }

    Move move(selectedSquare.x, selectedSquare.y, file, rank);

    // Handle promotion
    if (dynamic_cast<Pawn*>(movingPiece) != nullptr &&
        ((movingPiece->isWhite() && rank == 0) ||
            (!movingPiece->isWhite() && rank == 7)))
    {
        bool isMovingStraight = (selectedSquare.x == file);

        if ((movingPiece->isWhite() && selectedSquare.y == 1 && rank == 0) ||
            (!movingPiece->isWhite() && selectedSquare.y == 6 && rank == 7))
        {
            if (!isMovingStraight || (isMovingStraight && gameState.getPieceAt(file, rank) == nullptr))
            {
                char promotion = 0;
                int key = 0;
                while (!key)
                {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText("Promote pawn: Press Q, R, N, or B", 1080 / 4, 1080 / 2, 40, WHITE);
                    EndDrawing();
                    key = GetKeyPressed();
                    if (key != KEY_Q && key != KEY_R && key != KEY_N && key != KEY_B)
                        key = 0;
                }
                switch (key)
                {
                case KEY_Q: promotion = 'q'; break;
                case KEY_R: promotion = 'r'; break;
                case KEY_N: promotion = 'n'; break;
                case KEY_B: promotion = 'b'; break;
                }
                move = Move(selectedSquare.x, selectedSquare.y, file, rank, promotion);
            }
        }
    }

    // Validate move
    bool valid = false;

    for (int i = 0; i < possibleMoves.size(); i++) {
        if (possibleMoves[i] == move) {
            valid = true;
            break;
        }
    }

    if (valid)
    {
		GameState previousState(gameState);
		previousStates.push(previousState);
		if (!nextStates.empty()) {
			nextStates = std::stack<GameState>();
		}
        
        gameState.applyMove(move);
		lastMove = move;
        std::cout << "Evaluation value: " << gameState.evaluationValue(true) << "\n";
    }

    // Reset selection
    selectedSquare = { -1, -1 };
    possibleMoves.clear();
}

void drawBoard(const GameState& gameState, const std::vector<Move>& possibleMoves, const Vector2& selectedSquare, const Move& lastMove, const std::unordered_map<std::string, Texture2D>& textures, int boardSize, int boardOffsetX, int boardOffsetY, bool isFlipped) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Define new colors
    Color brown = { 168, 126, 98, 255 }; // Brown color
    Color offWhite = { 244, 222, 203, 255 }; // Off-white color
    Color textColor = BLACK; // Coordinate text color
    Color flipIndicatorColor = YELLOW; // Color for the flip indicator button

    // Calculate text size based on board size
    int textSize = boardSize / 40;
    
    // Draw the flip indicator/button
    int buttonSize = boardSize / 16;
    Rectangle flipButton = { 
        (float)(boardOffsetX + boardSize - buttonSize - 10), 
        (float)(boardOffsetY - buttonSize - 10), 
        (float)buttonSize, 
        (float)buttonSize 
    };
    DrawRectangleRec(flipButton, flipIndicatorColor);
    DrawRectangleLinesEx(flipButton, 2, BLACK);
    DrawText("F", flipButton.x + buttonSize/4, flipButton.y + buttonSize/4, buttonSize/2, BLACK);
    
    // Draw orientation indicator text
    const char* orientationText = isFlipped ? "Black at Bottom (F to Flip)" : "White at Bottom (F to Flip)";
    DrawText(orientationText, boardOffsetX, boardOffsetY - buttonSize - 5, textSize, WHITE);

    // Draw chess board
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            // Calculate visual position (which may be flipped)
            int visualX = isFlipped ? 7 - x : x;
            int visualY = isFlipped ? 7 - y : y;
            
            Color color = (visualX + visualY) % 2 ? brown : offWhite;
            DrawRectangle(boardOffsetX + visualX * (boardSize / 8), boardOffsetY + visualY * (boardSize / 8), (boardSize / 8), (boardSize / 8), color);

            // Draw coordinates inside the squares - use actual chess coordinates
            char fileChar = 'A' + x;
            int rankNum = 8 - y;
            std::string coordinate = std::string(1, fileChar) + std::to_string(rankNum);
            DrawText(coordinate.c_str(), 
                     boardOffsetX + visualX * (boardSize / 8) + 5, 
                     boardOffsetY + (visualY + 1) * (boardSize / 8) - textSize - 5, 
                     textSize, 
                     textColor);
        }
    }

    // Highlight the last move
    if (lastMove.x1() != 0 || lastMove.x2() != 0 || lastMove.y1() != 0 || lastMove.y2() != 0) 
    {
		DrawRectangle(boardOffsetX + lastMove.x1() * (boardSize / 8), boardOffsetY + lastMove.y1() * (boardSize / 8), (boardSize / 8), (boardSize / 8), ColorAlpha(BLUE, 0.5f));
		DrawRectangle(boardOffsetX + lastMove.x2() * (boardSize / 8), boardOffsetY + lastMove.y2() * (boardSize / 8), (boardSize / 8), (boardSize / 8), ColorAlpha(BLUE, 0.5f));
    }

    // Highlight possible moves
    for (const auto& move : possibleMoves)
    {
        int x = move.x2();
        int y = move.y2();
        
        // Apply flipping for visualization
        int visualX = isFlipped ? 7 - x : x;
        int visualY = isFlipped ? 7 - y : y;
        
        DrawRectangle(boardOffsetX + visualX * (boardSize / 8), boardOffsetY + visualY * (boardSize / 8), (boardSize / 8), (boardSize / 8), ColorAlpha(GREEN, 0.3f));
    }

    // Highlight selected square
    if (selectedSquare.x != -1)
    {
        int visualX = isFlipped ? 7 - (int)selectedSquare.x : (int)selectedSquare.x;
        int visualY = isFlipped ? 7 - (int)selectedSquare.y : (int)selectedSquare.y;
        
        Color color = gameState.getPieceAt(selectedSquare.x, selectedSquare.y) != 0 && gameState.getPieceAt(selectedSquare.x, selectedSquare.y)->isWhite() == gameState.isWhiteSideToMove() ? YELLOW : RED;
        DrawRectangle(boardOffsetX + visualX * (boardSize / 8), boardOffsetY + visualY * (boardSize / 8), (boardSize / 8), (boardSize / 8), ColorAlpha(color, 0.5f));
    }

    // Draw pieces
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Piece* piece = gameState.getPieceAt(x, y);
            drawPiece(piece, x, y, textures, (boardSize / 8), boardOffsetX, boardOffsetY, isFlipped);
        }
    }

    EndDrawing();
}

void drawPiece(Piece* piece, int x, int y, const std::unordered_map<std::string, Texture2D>& textures, int squareSize, int offsetX, int offsetY, bool isFlipped)
{
    if (!piece)
        return;
        
    // Apply flipping for visualization
    int visualX = isFlipped ? 7 - x : x;
    int visualY = isFlipped ? 7 - y : y;

    char pieceChar = ' ';
    if (dynamic_cast<Queen*>(piece))
        pieceChar = 'Q';
    else if (dynamic_cast<Pawn*>(piece))
        pieceChar = 'P';
    else if (dynamic_cast<Rook*>(piece))
        pieceChar = 'R';
    else if (dynamic_cast<Knight*>(piece))
        pieceChar = 'N';
    else if (dynamic_cast<Bishop*>(piece))
        pieceChar = 'B';
    else if (dynamic_cast<King*>(piece))
        pieceChar = 'K';

    std::string key = std::string(1, pieceChar) + (piece->isWhite() ? "W" : "B");
    Texture2D texture = textures.at(key);

    Rectangle sourceRec = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle destRec = { offsetX + visualX * squareSize, offsetY + visualY * squareSize, squareSize, squareSize };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}
