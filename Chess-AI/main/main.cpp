#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "raylib.h"
#include "gameState/gameState.h"
#include "gameState/currentGameState.h"
#include "pieces/bishop.h"
#include "pieces/rook.h"
#include "pieces/queen.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/king.h"
#include "move.h"

// Constants
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 1080;
const float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

// Helper function to convert indices to chess notation
std::string indexToCoord(int x, int y)
{
    return std::string(1, char('a' + x)) + std::string(1, char('8' - y));
}

// Helper function to convert screen coordinates to board indices
void screenToBoard(int mouseX, int mouseY, int& boardX, int& boardY, int squareSize)
{
    boardX = mouseX / squareSize;
    boardY = mouseY / squareSize;
}

// Helper function to load textures (you'll need to provide actual image files)
Texture2D loadPieceTexture(char pieceType, bool isWhite)
{
    std::string path = "main/resources/";
    path += isWhite ? "white_" : "black_";

    switch (toupper(pieceType))
    {
    case 'P':
        path += "pawn.png";
        break;
    case 'R':
        path += "rook.png";
        break;
    case 'N':
        path += "knight.png";
        break;
    case 'B':
        path += "bishop.png";
        break;
    case 'Q':
        path += "queen.png";
        break;
    case 'K':
        path += "king.png";
        break;
    }
    return LoadTexture(path.c_str());
}

// Helper function to draw pieces
void drawPiece(Piece* piece, int x, int y, const std::unordered_map<std::string, Texture2D>& textures, int squareSize)
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

    // Calculate the scale factor
    float scaleFactor = (float)squareSize / texture.width;

    // Define the source and destination rectangles
    Rectangle sourceRec = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle destRec = { x * squareSize, y * squareSize, squareSize, squareSize };
    Vector2 origin = { 0, 0 };

    // Draw the texture with scaling
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    // Raylib initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess Game");
    // Window configuration flags
    SetTargetFPS(60);

    CurrentGameState gameState;
    Vector2 selectedSquare = { -1, -1 };
    std::vector<Move> possibleMoves;

    // Load textures once and store them in a map
    std::unordered_map<std::string, Texture2D> textures;
    textures["PW"] = loadPieceTexture('P', true);
    textures["PB"] = loadPieceTexture('P', false);
    textures["RW"] = loadPieceTexture('R', true);
    textures["RB"] = loadPieceTexture('R', false);
    textures["NW"] = loadPieceTexture('N', true);
    textures["NB"] = loadPieceTexture('N', false);
    textures["BW"] = loadPieceTexture('B', true);
    textures["BB"] = loadPieceTexture('B', false);
    textures["QW"] = loadPieceTexture('Q', true);
    textures["QB"] = loadPieceTexture('Q', false);
    textures["KW"] = loadPieceTexture('K', true);
    textures["KB"] = loadPieceTexture('K', false);

    while (!WindowShouldClose())
    {
        // Get current window size
        int currentWidth = GetScreenWidth();
        int currentHeight = GetScreenHeight();
        int squareSize = currentWidth / 8;

        // Input handling
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            int file = mousePos.x / squareSize;
            int rank = mousePos.y / squareSize;

            if (selectedSquare.x == -1)
            { // First click
                Piece* selectedPiece = gameState.getPieceAt(file, rank);
                if (selectedPiece)
                {
                    selectedSquare = { (float)file, (float)rank };
                    // Get possible moves
                    selectedPiece->possibleMoves(possibleMoves, file, rank, gameState);
                }
            }
            else
            { // Second click
                Piece* movingPiece = gameState.getPieceAt(selectedSquare.x, selectedSquare.y);
                // Create move normally (without promotion letter)
                Move move(selectedSquare.x, selectedSquare.y, file, rank);

                // If the pawn reaches the last row, prompt and recreate Move with promotion letter
                bool isPromotionMove = false;
                if (dynamic_cast<Pawn*>(movingPiece) != nullptr &&
                    ((movingPiece->isWhite() && rank == 0) ||
                        (!movingPiece->isWhite() && rank == 7)))
                {
                    // Check if the pawn is moving straight or diagonally
                    bool isMovingStraight = (selectedSquare.x == file);

                    // Only allow promotion if the pawn is actually moving to the last rank
                    // and either moving diagonally (capture) or moving straight to an empty square
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
                                ClearBackground(RAYWHITE);
                                DrawText("Promote pawn: Press Q, R, N, or B", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 40, BLACK);
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
                            isPromotionMove = true;
                        }
                    }
                }
                // Validate move only if not a promotion move
                bool valid = false;
                if (!isPromotionMove)
                {
                    GameState newGameState(gameState);
                    newGameState.applyMove(move);
                    auto possibleStates = gameState.possibleNewGameStates(movingPiece->isWhite());
                    for (const auto& state : possibleStates)
                    {
                        if (state == newGameState)
                        {
                            valid = true;
                            break;
                        }
                    }
                }
                else
                {
                    valid = true;
                }
                if (valid)
                {
                    gameState.applyMove(move);
                }
                // Reset selection
                selectedSquare = { -1, -1 };
                possibleMoves.clear();
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw chess board
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                Color color = (x + y) % 2 ? DARKGRAY : LIGHTGRAY;
                DrawRectangle(x * squareSize, y * squareSize, squareSize, squareSize, color);
            }
        }

        // Draw coordinates on left (row numbers)
        for (int i = 0; i < 8; i++) {
            std::string rowNum = std::to_string(8 - i);
            DrawText(rowNum.c_str(), 5, i * squareSize + squareSize / 2 - 10, 40, BLACK);
        }

        // Draw coordinates at the bottom (column letters)
        for (int i = 0; i < 8; i++) {
            char letter = 'A' + i;
            std::string colLetter(1, letter);
            int textWidth = MeasureText(colLetter.c_str(), 20);
            DrawText(colLetter.c_str(), i * squareSize + squareSize / 2 - textWidth / 2, currentHeight - 25, 40, BLACK);
        }

        // Highlight possible moves
        for (const auto& move : possibleMoves)
        {
            int x = move.x2();
            int y = move.y2();
            DrawRectangle(x * squareSize, y * squareSize, squareSize, squareSize,
                ColorAlpha(GREEN, 0.3f));
        }

        // Highlight selected square
        if (selectedSquare.x != -1)
        {
            DrawRectangle(selectedSquare.x * squareSize, selectedSquare.y * squareSize,
                squareSize, squareSize, ColorAlpha(YELLOW, 0.3f));
        }

        // Draw pieces
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                Piece* piece = gameState.getPieceAt(x, y);
                drawPiece(piece, x, y, textures, squareSize);
            }
        }

        EndDrawing();
    }

    // Unload textures
    for (auto& texture : textures)
    {
        UnloadTexture(texture.second);
    }

    CloseWindow();
    return 0;
}
