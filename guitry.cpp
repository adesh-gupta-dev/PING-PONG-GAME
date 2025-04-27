#include <raylib.h>

const int screenWidth = 1000;
const int screenHeight = 550;

enum GameState { HOME, GAME, GAME_OVER, WIN };

class Paddle {
public:
    Rectangle rect;
    int speed;

    Paddle(float x, float y, float width, float height, int speed) {
        rect = { x, y, width, height };
        this->speed = speed;
    }

    void MoveUp(float deltaTime) {
        if (rect.y > 0) rect.y -= speed * deltaTime;
    }

    void MoveDown(float deltaTime) {
        if (rect.y < screenHeight - rect.height) rect.y += speed * deltaTime;
    }
};

class Ball {
public:
    Vector2 position;
    Vector2 speed;
    float radius;

    Ball(float x, float y, float radius, float speedX, float speedY) {
        position = { x, y };
        speed = { speedX, speedY };
        this->radius = radius;
    }

    void Move(float deltaTime) {
        position.x += speed.x * deltaTime;
        position.y += speed.y * deltaTime;
    }

    void CheckCollisionWithWalls() {
        if (position.y <= radius || position.y >= screenHeight - radius) speed.y *= -1;
    }

    void Reset() {
        position = { screenWidth / 2, screenHeight / 2 };
        speed = { 300, 300 };
    }
};

class Score {
public:
    int left;
    int right;

    Score() {
        left = 0;
        right = 0;
    }

    void Reset() {
        left = 0;
        right = 0;
    }
};

class Game {
private:
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;
    Score score;
    GameState state;
    int difficulty;

public:
    Game() : leftPaddle(50, screenHeight / 2 - 50, 10, 100, 500), rightPaddle(screenWidth - 60, screenHeight / 2 - 50, 10, 100, 500), ball(screenWidth / 2, screenHeight / 2, 10, 300, 300), score() {
        state = HOME;
        difficulty = 1; // 1: Easy, 2: Medium, 3: Hard
    }

    void Init() {
        InitWindow(screenWidth, screenHeight, "Ping Pong Game");
        SetTargetFPS(50);
    }

    void Update(float deltaTime) {
        switch (state) {
        case HOME:
            UpdateHomeScreen();
            break;
        case GAME:
            UpdateGameScreen(deltaTime);
            break;
        case GAME_OVER:
            UpdateGameOverScreen();
            break;
        case WIN:
            UpdateWinScreen();
            break;
        }
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(BLACK);

        switch (state) {
        case HOME:
            DrawHomeScreen();
            break;
        case GAME:
            DrawGameScreen();
            break;
        case GAME_OVER:
            DrawGameOverScreen();
            break;
        case WIN:
            DrawWinScreen();
            break;
        }

        EndDrawing();
    }

    void Close() {
        CloseWindow();
    }

private:
    void UpdateHomeScreen() {
        if (IsKeyPressed(KEY_ONE)) difficulty = 1;
        if (IsKeyPressed(KEY_TWO)) difficulty = 2;
        if (IsKeyPressed(KEY_THREE)) difficulty = 3;
        if (IsKeyPressed(KEY_ENTER)) state = GAME;
    }

    void UpdateGameScreen(float deltaTime) {
        if (IsKeyDown(KEY_UP)) leftPaddle.MoveUp(deltaTime);
        if (IsKeyDown(KEY_DOWN)) leftPaddle.MoveDown(deltaTime);

        if (ball.position.y < rightPaddle.rect.y) rightPaddle.MoveUp(deltaTime);
        if (ball.position.y > rightPaddle.rect.y + rightPaddle.rect.height) rightPaddle.MoveDown(deltaTime);

        ball.Move(deltaTime);
        ball.CheckCollisionWithWalls();

        if (CheckCollisionCircleRec(ball.position, ball.radius, leftPaddle.rect) || CheckCollisionCircleRec(ball.position, ball.radius, rightPaddle.rect)) {
            ball.speed.x *= -1;
        }

        if (ball.position.x <= 0) {
            score.right++;
            ball.Reset();
        }
        if (ball.position.x >= screenWidth) {
            score.left++;
            ball.Reset();
        }

        if (score.left >= 10) state = WIN;
        if (score.right >= 10) state = GAME_OVER;
    }

    void UpdateGameOverScreen() {
        if (IsKeyPressed(KEY_ENTER)) {
            score.Reset();
            state = HOME;
        }
    }

    void UpdateWinScreen() {
        if (IsKeyPressed(KEY_ENTER)) {
            score.Reset();
            state = HOME;
        }
    }

    void DrawHomeScreen() {
        DrawText("PING PONG GAME", screenWidth / 2 - MeasureText("PING PONG GAME", 40) / 2, screenHeight / 2 - 50, 40, WHITE);
        DrawText("Press 1 for Easy", screenWidth / 2 - MeasureText("Press 1 for Easy", 20) / 2, screenHeight / 2, 20, WHITE);
        DrawText("Press 2 for Medium", screenWidth / 2 - MeasureText("Press 2 for Medium", 20) / 2, screenHeight / 2 + 30, 20, WHITE);
        DrawText("Press 3 for Hard", screenWidth / 2 - MeasureText("Press 3 for Hard", 20) / 2, screenHeight / 2 + 60, 20, WHITE);
        DrawText("Press ENTER to Start", screenWidth / 2 - MeasureText("Press ENTER to Start", 20) / 2, screenHeight / 2 + 90, 20, WHITE);
    }

    void DrawGameScreen() {
		DrawRectangle(0, 0, screenWidth/2, screenHeight, GREEN);
        DrawRectangle(screenWidth/2, 0, screenWidth , screenHeight, DARKGREEN);
		DrawRectangle(screenWidth / 2 - 5, 0, 10, screenHeight, WHITE);
		DrawCircle(screenWidth / 2, screenHeight / 2, 50, WHITE);
        DrawCircle(screenWidth / 2, screenHeight / 2, 48, DARKGREEN);
        DrawRectangleRec(leftPaddle.rect, WHITE);
        DrawRectangleRec(rightPaddle.rect, WHITE);
        DrawCircleV(ball.position, ball.radius, WHITE);
	/*	DrawLine(screenWidth/2,0, screenWidth/2, screenHeight,WHITE);*/
        DrawText(TextFormat("%d", score.left), screenWidth / 4, 20, 40, WHITE);
        DrawText(TextFormat("%d", score.right), 3 * screenWidth / 4, 20, 40, WHITE);
    }

    void DrawGameOverScreen() {
        DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 50, 40, WHITE);
        DrawText("Press ENTER to Return to Home", screenWidth / 2 - MeasureText("Press ENTER to Return to Home", 20) / 2, screenHeight / 2, 20, WHITE);
    }

    void DrawWinScreen() {
        DrawText("YOU WIN!", screenWidth / 2 - MeasureText("YOU WIN!", 40) / 2, screenHeight / 2 - 50, 40, WHITE);
        DrawText("Press ENTER to Return to Home", screenWidth / 2 - MeasureText("Press ENTER to Return to Home", 20) / 2, screenHeight / 2, 20, WHITE);
    }
};

int main() {
    Game game;
    game.Init();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        game.Update(deltaTime);
        game.Draw();
    }

    game.Close();
    return 0;
}
