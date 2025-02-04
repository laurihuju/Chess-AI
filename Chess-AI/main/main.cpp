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
const int SQUARE_SIZE = SCREEN_WIDTH / 8;

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
void drawPiece(Piece* piece, int x, int y, const std::unordered_map<std::string, Texture2D>& textures)
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
	DrawTexture(texture, x * SQUARE_SIZE, y * SQUARE_SIZE, WHITE);
}

int main()
{
	// Raylib initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess Game");
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
		// Input handling
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			Vector2 mousePos = GetMousePosition();
			int file = mousePos.x / SQUARE_SIZE;
			int rank = mousePos.y / SQUARE_SIZE;

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
                if (dynamic_cast<Pawn*>(movingPiece) != nullptr &&
                    ((movingPiece->isWhite() && rank == 0) ||
                     (!movingPiece->isWhite() && rank == 7)))
                {
                    char promotion = 0;
                    int key = 0;
                    while (!key)
                    {
                        BeginDrawing();
                            ClearBackground(RAYWHITE);
                            DrawText("Promote pawn: Press Q, R, N, or B", SCREEN_WIDTH/4, SCREEN_HEIGHT/2, 40, BLACK);
                        EndDrawing();
                        key = GetKeyPressed();
                        if (key != KEY_Q && key != KEY_R && key != KEY_N && key != KEY_B)
                            key = 0;
                    }
                    switch(key)
                    {
                        case KEY_Q: promotion = 'q'; break;
                        case KEY_R: promotion = 'r'; break;
                        case KEY_N: promotion = 'n'; break;
                        case KEY_B: promotion = 'b'; break;
                    }
                    move = Move(selectedSquare.x, selectedSquare.y, file, rank, promotion);
                }
				// Validate move
				GameState newGameState(gameState);
				newGameState.applyMove(move);
				bool valid = false;
				auto possibleStates = gameState.possibleNewGameStates(movingPiece->isWhite());
				for (const auto& state : possibleStates)
				{
					if (state == newGameState)
					{
						valid = true;
						break;
					}
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
				DrawRectangle(x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, color);
			}
		}

		// Highlight possible moves
		for (const auto& move : possibleMoves)
		{
			int x = move.x2();
			int y = move.y2();
			DrawRectangle(x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE,
				ColorAlpha(GREEN, 0.3f));
		}

		// Highlight selected square
		if (selectedSquare.x != -1)
		{
			DrawRectangle(selectedSquare.x * SQUARE_SIZE, selectedSquare.y * SQUARE_SIZE,
				SQUARE_SIZE, SQUARE_SIZE, ColorAlpha(YELLOW, 0.3f));
		}

		// Draw pieces
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				Piece* piece = gameState.getPieceAt(x, y);
				drawPiece(piece, x, y, textures);
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
