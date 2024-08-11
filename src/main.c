#include "raylib.h"
#include "raymath.h"
#include <stdio.h>


#define BLOCK_ROWS 5
#define BLOCK_COLUMNS 10
#define BLOCK_WIDTH 40
#define BLOCK_HEIGHT 15
#define BLOCK_PADDING 10


 typedef struct Ball{
        Vector2 position;
        Rectangle hitBox;
        float radius;
        bool isFalling;
        Color color;
    } Ball;

    typedef struct Paddle{
        Vector2 position;
        Rectangle hitBox1;
        Rectangle hitBox2;
        float width;
        float height;
        Color color;
    } Paddle;
    
    typedef struct block{
        Vector2 position;
        Rectangle hitBox;
        float width;
        float height;
        Color color;
        bool isDestroyed;
    } Block;
    
void update(Paddle *paddle, Ball *ball);
Block blocks[BLOCK_ROWS * BLOCK_COLUMNS];

void InitBlocks(){
    for (int row = 0; row < 5; row++){
        for (int col = 0; col < 10; col++){
            Block *block = &blocks[row * BLOCK_COLUMNS + col];
            block->position = (Vector2){col * (BLOCK_WIDTH + BLOCK_PADDING) + 5, row * (BLOCK_HEIGHT + BLOCK_PADDING) + 5};
            block->width = BLOCK_WIDTH;
            block->height = BLOCK_HEIGHT;
            block->color = BLUE;
            block->isDestroyed = false;
        }
    }
}

void DrawBlocks() {
    for (int i = 0; i < BLOCK_ROWS * BLOCK_COLUMNS; i++) {
        if (!blocks[i].isDestroyed) {
            DrawRectangleV(blocks[i].position, (Vector2){blocks[i].width, blocks[i].height}, blocks[i].color);
        }
    }
}
void CheckBallCollision(Ball *ball) {
    for (int i = 0; i < BLOCK_ROWS * BLOCK_COLUMNS; i++) {
        if (!blocks[i].isDestroyed) {
            Rectangle blockRect = {blocks[i].position.x, blocks[i].position.y, blocks[i].width, blocks[i].height};
            if (CheckCollisionCircleRec(ball->position, ball->radius, blockRect)) {
                blocks[i].isDestroyed = true;
                ball->isFalling = !ball->isFalling; // Change ball direction
                break;
            }
        }
    }
}

int main(void){
    InitWindow(500, 700, "Raylib Window");
   
    Paddle paddle = {
        .position = {GetScreenWidth()/2, GetScreenHeight()/1.2},
        .hitBox1 = {paddle.position.x, paddle.height,paddle.width/2, paddle.position.y},
        .hitBox2 = {paddle.position.x + paddle.width/2, paddle.height,paddle.width/2, paddle.position.y},
        .width = 70,
        .height = 25,
        .color = RED
    };
    
    Ball ball = {
        .position = {GetScreenWidth()/2, GetScreenHeight()/5},
        .radius = 10,
        .hitBox = {ball.radius,ball.radius, ball.position.x, ball.position.y},
        .isFalling = true,
        .color = BLUE
    };

    InitBlocks();

    for (int i = 0; i < 10; i++){
        Block block = {
            .position = {i * 50, 50},
            .hitBox = {block.position.x, block.position.y, 50, 25},
            .width = 50,
            .height = 25,
            .color = GREEN,
            .isDestroyed = false
        };
    }

    int randomStartValue = GetRandomValue(0, 1) == 0 ? -1 : 1;
    
    while(!WindowShouldClose()){

        //move paddle
        if (IsKeyDown(KEY_RIGHT)){
            if (paddle.position.x < GetScreenWidth() - paddle.width){
                paddle.position.x += GetFrameTime() * 300;
            }
        }
        if (IsKeyDown(KEY_LEFT)){
            if (paddle.position.x > 0){
                paddle.position.x -= GetFrameTime() * 300;
            }
        }


        //move ball
        ball.position.y +=  ball.isFalling ? GetFrameTime() * 400 : -GetFrameTime() * 400;
        ball.position.x += randomStartValue * (GetFrameTime() * 300);
        //align hitbox 
        update(&paddle, &ball);

        

        if (CheckCollisionRecs(ball.hitBox, paddle.hitBox1)){
            //if hits left half from right direction go left
            ball.isFalling = false;
            if (randomStartValue > 0){
                randomStartValue = -randomStartValue;
            }
            
        };
        if (CheckCollisionRecs(ball.hitBox, paddle.hitBox2)){
            //if hits right half from left direction go right
            ball.isFalling = false;
            if (randomStartValue < 0){
                randomStartValue = -randomStartValue;
            }
            
        };
        CheckBallCollision(&ball);

        if (ball.hitBox.y <= 0){
            ball.isFalling = !ball.isFalling;
        }
        if (ball.hitBox.x <= 0 || ball.hitBox.x >= GetScreenWidth() - ball.radius){
            randomStartValue = -randomStartValue;
        }


        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(paddle.position.x, GetScreenHeight()/1.2, paddle.width, 25, RED);
            DrawCircleV(ball.position, ball.radius, ball.color);
            DrawBlocks();
            
            // DrawCircle(ballX, ballY, 10, BLUE);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}

void update(Paddle *paddle, Ball *ball) {
    paddle->hitBox1.x = paddle->position.x;
    paddle->hitBox1.y = paddle->position.y;
    paddle->hitBox1.width = paddle->width/2;
    paddle->hitBox1.height = paddle->height;

    paddle->hitBox2.x = paddle->position.x + paddle->width/2;
    paddle->hitBox2.y = paddle->position.y;
    paddle->hitBox2.width = paddle->width/2;
    paddle->hitBox2.height = paddle->height;


    ball->hitBox.x = ball->position.x - ball->radius;
    ball->hitBox.y = ball->position.y - ball->radius;
    ball->hitBox.width = ball->radius * 2;
    ball->hitBox.height = ball->radius * 2;
}