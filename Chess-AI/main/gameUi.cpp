#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <iostream>

#include "raylib.h"

#include "gameState/gameState.h"
#include "gameState/currentGameState.h"
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
/// Checks input in the UI and handles updating the game state, turn, selected square and possible moves.
/// </summary>
/// <param name="gameState">The current game state</param>
/// <param name="turn">The current turn (true is white)</param>
/// <param name="selectedSquare">The selected square coordinates</param>
/// <param name="possibleMoves">The vector of possible moves</param>
/// <param name="boardSize">The board width and height</param>
/// <param name="boardOffsetX">The board X offset from the window 0 coordinate</param>
/// <param name="boardOffsetY">The board Y offset from the window 0 coordinate</param>
void handleInput(GameState& gameState, bool& turn, Vector2& selectedSquare, std::vector<Move>& possibleMoves, int boardSize, int boardOffsetX, int boardOffsetY);

/// <summary>
/// Draws the board content to the window.
/// </summary>
/// <param name="gameState">The current game state</param>
/// <param name="turn">The current turn (true is white)</param>
/// <param name="possibleMoves">The vector of possible moves</param>
/// <param name="selectedSquare">The selected square coordinates</param>
/// <param name="textures">Textures loaded with loadPieceTextures()</param>
/// <param name="boardSize">The board width and height</param>
/// <param name="boardOffsetX">The board X offset from the window 0 coordinate</param>
/// <param name="boardOffsetY">The board Y offset from the window 0 coordinate</param>
void drawBoard(const GameState& gameState, const bool& turn, const std::vector<Move>& possibleMoves, const Vector2& selectedSquare, const std::unordered_map<std::string, Texture2D>& textures, int boardSize, int boardOffsetX, int boardOffsetY);

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
void drawPiece(Piece* piece, int x, int y, const std::unordered_map<std::string, Texture2D>& textures, int squareSize, int offsetX, int offsetY);

void startGameUi()
{
    // Initialize the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1080, 1080, "Chess Game");
    SetTargetFPS(60);
    MaximizeWindow();

    // The current game state and turn
    CurrentGameState gameState;
    bool turn = true;

    // The selected square and possible moves
    Vector2 selectedSquare = { -1, -1 };
    std::vector<Move> possibleMoves;

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
        handleInput(gameState, turn, selectedSquare, possibleMoves, boardSize, boardOffsetX, boardOffsetY);
        
        // Render the board
        drawBoard(gameState, turn, possibleMoves, selectedSquare, textures, boardSize, boardOffsetX, boardOffsetY);
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

void handleInput(GameState& gameState, bool& turn, Vector2& selectedSquare, std::vector<Move>& possibleMoves, int boardSize, int boardOffsetX, int boardOffsetY) {
    // Use AI to complete the move when pressing space
    if (IsKeyPressed(KEY_SPACE)) {
        // Find the best move and calculate the calculation time
        auto startTime = std::chrono::high_resolution_clock::now();
        Move aiMove = ChessAI::findBestMove(gameState, turn, 4); // Depth 4 is the best that has an acceptable runtime.
        auto endTime = std::chrono::high_resolution_clock::now();

        // Print debugging data
        std::cout << "Calculation time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms\n";
        std::cout << (turn ? "White" : "Black") << ": (" << aiMove.x1() << "; " << aiMove.y1() << ") -> (" << aiMove.x2() << "; " << aiMove.y2() << ")" << "\n";

        // If AI returned an empty move, there are no moves available
        if (aiMove == Move(0, 0, 0, 0)) {
            return;
        }

        gameState.applyMove(aiMove);
        turn = !turn;
        return;
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

    // First click (select piece)
    if (selectedSquare.x == -1)
    {
        Piece* selectedPiece = gameState.getPieceAt(file, rank);
        if (selectedPiece)
        {
            selectedSquare = { (float)file, (float)rank };

            // There are no possible moves if moving piece at wrong turn
            if (selectedPiece->isWhite() == turn) {
                selectedPiece->possibleMoves(possibleMoves, file, rank, gameState);

                // Do not show moves that would lead the player to check
                for (int i = 0; i < possibleMoves.size(); i++) {
                    GameState testState(gameState);
                    testState.applyMove(possibleMoves[i]);

                    if (testState.isCheck(turn)) {
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
    if (movingPiece != 0 && movingPiece->isWhite() != turn) {
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
        gameState.applyMove(move);
        turn = !turn;

        std::cout << "Evaluation value: " << gameState.evaluate(true) << "\n";
    }

    // Reset selection
    selectedSquare = { -1, -1 };
    possibleMoves.clear();
}

void drawBoard(const GameState& gameState, const bool& turn, const std::vector<Move>& possibleMoves, const Vector2& selectedSquare, const std::unordered_map<std::string, Texture2D>& textures, int boardSize, int boardOffsetX, int boardOffsetY) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw chess board
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Color color = (x + y) % 2 ? DARKGRAY : LIGHTGRAY;
            DrawRectangle(boardOffsetX + x * (boardSize / 8), boardOffsetY + y * (boardSize / 8), (boardSize / 8), (boardSize / 8), color);
        }
    }

    // Draw coordinates on left (row numbers)
    for (int i = 0; i < 8; i++) {
        std::string rowNum = std::to_string(8 - i);
        DrawText(rowNum.c_str(), boardOffsetX - 20, boardOffsetY + i * (boardSize / 8) + (boardSize / 8) / 2 - 10, 20, WHITE);
    }

    // Draw coordinates at the bottom (column letters)
    for (int i = 0; i < 8; i++) {
        char letter = 'A' + i;
        std::string colLetter(1, letter);
        int textWidth = MeasureText(colLetter.c_str(), 20);
        DrawText(colLetter.c_str(), boardOffsetX + i * (boardSize / 8) + (boardSize / 8) / 2 - textWidth / 2, boardOffsetY + boardSize + 5, 20, WHITE);
    }

    // Highlight possible moves
    for (const auto& move : possibleMoves)
    {
        int x = move.x2();
        int y = move.y2();
        DrawRectangle(boardOffsetX + x * (boardSize / 8), boardOffsetY + y * (boardSize / 8), (boardSize / 8), (boardSize / 8), ColorAlpha(GREEN, 0.3f));
    }

    // Highlight selected square
    if (selectedSquare.x != -1)
    {
        Color color = gameState.getPieceAt(selectedSquare.x, selectedSquare.y) != 0 && gameState.getPieceAt(selectedSquare.x, selectedSquare.y)->isWhite() == turn ? YELLOW : RED;
        DrawRectangle(boardOffsetX + selectedSquare.x * (boardSize / 8), boardOffsetY + selectedSquare.y * (boardSize / 8), (boardSize / 8), (boardSize / 8), ColorAlpha(color, 0.3f));
    }

    // Draw pieces
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Piece* piece = gameState.getPieceAt(x, y);
            drawPiece(piece, x, y, textures, (boardSize / 8), boardOffsetX, boardOffsetY);
        }
    }

    EndDrawing();
}

void drawPiece(Piece* piece, int x, int y, const std::unordered_map<std::string, Texture2D>& textures, int squareSize, int offsetX, int offsetY)
{
    if (!piece)
        return;

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
    Rectangle destRec = { offsetX + x * squareSize, offsetY + y * squareSize, squareSize, squareSize };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}
