#include "raylib.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

const int MAX_ROWS = 10;
const int MAX_COLS = 10;
const int TILE_SIZE = 60;
const int GAPS = 10;

int currentRows = 6;
int currentCols = 5;

vector<int> checkWord(string guess, string secret, int cols) {
    vector<int> result(cols, 0);
    vector<bool> used(cols, false);
    for (int i = 0; i < cols; i++) {
        if (guess[i] == secret[i]) {
            result[i] = 2;
            used[i] = true;
        }
    }
    for (int i = 0; i < cols; i++) {
        if (result[i] == 0) {
            for (int j = 0; j < cols; j++) {
                if (!used[j] && guess[i] == secret[j]) {
                    result[i] = 1;
                    used[j] = true;
                    break;
                }
            }
        }
    }
    return result;
}
Color myGreen = {150, 200, 100, 255};
bool Button(Rectangle rect, const char* text) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);
    Color color = hover ? GREEN : myGreen;
    DrawRectangleRec(rect, color);
    DrawRectangleLinesEx(rect, 2, WHITE);
    int textWidth = MeasureText(text, 20);
    DrawText(text, rect.x + rect.width / 2 - textWidth / 2, rect.y + rect.height / 2 - 10, 20, WHITE);
    return (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

int main() {
    InitWindow(800, 600, "Raylib Wordle");
    SetTargetFPS(60);

    vector<string> wordlist;
    srand(time(0));

    string secret;
    char letters[MAX_ROWS][MAX_COLS];
    int colors[MAX_ROWS][MAX_COLS];
    float tileScale[MAX_ROWS][MAX_COLS];

    int currentRow = 0;
    int currentCol = 0;
    bool gameOver = false;
    bool win = false;

    enum GameState { MENU, SETTING, GAME, GAMEOVER };
    GameState currentState = MENU;

    auto resetGame = [&]() {
        if (!wordlist.empty()) secret = wordlist[rand() % wordlist.size()];
        currentRow = 0;
        currentCol = 0;
        gameOver = false;
        win = false;
        for (int r = 0; r < MAX_ROWS; r++) {
            for (int c = 0; c < MAX_COLS; c++) {
                letters[r][c] = ' ';
                colors[r][c] = -1;
                tileScale[r][c] = 1.0f;
            }
        }
    };

    while (!WindowShouldClose()) {
        
        for (int r = 0; r < currentRows; r++) {
            for (int c = 0; c < currentCols; c++) {
                if (tileScale[r][c] > 1.0f) {
                    tileScale[r][c] -= 0.05f;
                    if (tileScale[r][c] < 1.0f) tileScale[r][c] = 1.0f;
                }
            }
        }

        if (currentState == GAME && !gameOver) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 'a' && key <= 'z') key -= 32;
                if (key >= 'A' && key <= 'Z' && currentCol < currentCols) {
                    letters[currentRow][currentCol] = (char)key;
                    tileScale[currentRow][currentCol] = 1.25f;
                    currentCol++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && currentCol > 0) {
                currentCol--;
                letters[currentRow][currentCol] = ' ';
            }

            if (IsKeyPressed(KEY_ENTER) && currentCol == currentCols) {
                string guess = "";
                for (int i = 0; i < currentCols; i++) guess += letters[currentRow][i];

                vector<int> result = checkWord(guess, secret, currentCols);
                bool allGreen = true;
                for (int i = 0; i < currentCols; i++) {
                    colors[currentRow][i] = result[i];
                    if (result[i] != 2) allGreen = false;
                }

                if (allGreen) { win = true; gameOver = true; }
                else {
                    currentRow++;
                    currentCol = 0;
                    if (currentRow == currentRows) gameOver = true;
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (currentState == MENU) {
            DrawText("WORDLE", 322, 100, 40, WHITE);
            if (Button({ 300, 250, 200, 60 }, "PLAY")) currentState = SETTING;
        } 
        else if (currentState == SETTING) {
            DrawText("SELECT DIFFICULTY", 250, 100, 30, GOLD);
            
            if (Button({ 300, 200, 200, 60 }, "EASY")) {
                currentRows = 6;
                currentCols = 4;
                wordlist = wordlist = {"BOOK", "FISH", "TREE", "MOON", "FIRE", 
                                       "DUCK", "STAR", "WIND", "CAKE", "SNOW", 
                                       "FROG", "BOAT", "BIRD", "LAKE", "COIN", 
                                       "DOOR", "KING", "LAMP", "MILK", "NEST", 
                                       "PARK", "RAIN", "SHIP", "TENT", "ZOOM"};
                resetGame();
                currentState = GAME;
            }
            if(Button({300, 280, 200, 60}, "MEDIUM")){
                currentRows = 6;
                currentCols = 5;
                wordlist = wordlist = {"APPLE", "TABLE", "HOUSE", "WATER", "SMILE", 
                                       "BREAD", "LIGHT", "GRAPE", "PLANT", "CHAIR",
                                       "CLOUD", "WATCH", "SHIRT", "PIZZA", "PIANO", 
                                       "STORM", "GLASS", "TRAIN", "DREAM", "HEART"};
                resetGame();
                currentState = GAME;
            }
            if(Button({300, 360, 200, 60}, "HARD")){
                currentRows = 6;
                currentCols = 5;
                wordlist = wordlist = wordlist = {"LYMPH", "QUART", "VOODO", "JAZZY", 
                                                  "KAYAK", "WAXEN", "ABYSS", "EPOXY", 
                                                  "HYENA", "QUIRK", "ZILCH", "GNOME", 
                                                  "FJORD", "IVORY", "VIXEN"};
                resetGame();
                currentState = GAME;
            }
            if(Button({300, 440, 200, 60}, "INSANE")){
                currentRows = 6;
                currentCols = 6;
                wordlist = wordlist = {"BANANA", "BOTTLE", "BRIDGE", "CANDLE", "DRAGON", 
                                       "FLOWER", "GARDEN", "HAMMER", "JACKET", "KITTEN", 
                                       "LAPTOP", "MARKET", "ORANGE", "PENCIL", "PLANET", 
                                       "ROCKET", "SCHOOL", "SILVER", "STREET", "TICKET", 
                                       "TURTLE", "WINDOW", "YELLOW", "ZIGZAG"};
                resetGame();
                currentState = GAME;
            }
        }
        else if (currentState == GAME || currentState == GAMEOVER) {
            
            float startX = (800 - (currentCols * (TILE_SIZE + GAPS))) / 2.0f;

            for (int r = 0; r < currentRows; r++) {
                for (int c = 0; c < currentCols; c++) {
                    float x = startX + c * (TILE_SIZE + GAPS);
                    float y = 100 + r * (TILE_SIZE + GAPS);
                    float currentS = tileScale[r][c];
                    float offset = (TILE_SIZE * currentS - TILE_SIZE) / 2.0f;

                    Color cellColor = DARKGRAY;
                    if (colors[r][c] == 2) cellColor = GREEN;
                    else if (colors[r][c] == 1) cellColor = YELLOW;
                    else if (colors[r][c] == 0) cellColor = GRAY;

                    Rectangle tileRect = { x - offset, y - offset, TILE_SIZE * currentS, TILE_SIZE * currentS };
                    DrawRectangleRec(tileRect, cellColor);
                    DrawRectangleLinesEx(tileRect, 2, (r == currentRow && !gameOver) ? WHITE : DARKGRAY);

                    char letter[2] = { letters[r][c], '\0' };
                    DrawText(letter, x + 20, y + 15, 30 * currentS, BLACK);
                }
            }

            if (gameOver) {
                DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.8f));
                if(win) DrawText("GENIUS!", 320, 150, 40, GREEN);
                if (!win){
                    DrawText("YOU LOSE", 300, 150, 40,RED);
                    DrawText(TextFormat("WORD WAS: %s", secret.c_str()), 315, 200, 20, WHITE);
                }
                
                if (Button({ 300, 300, 200, 60 }, "RETRY")) { resetGame(); currentState = GAME; }
                if (Button({ 300, 380, 200, 60 }, "MENU")) currentState = MENU;
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}