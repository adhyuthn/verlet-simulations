#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <ctime>
#include <random>

#define randMax(a) (rand() % a)

class Circle {
    public:
        float radius;
        Vector2 position;
        Vector2 velocity;
        Color color;
        float restitution;

        Circle(float radius, Vector2 x, Vector2 v, Color color):
            radius(radius),
            position(x),
            velocity(v),
            color(color)
        {
            restitution = 0.8 + randMax(19)/100.0;
        }

        void draw() {
            DrawCircle(position.x, position.y, radius, color);
        }

        void updatePosition(Vector2 acceleration, float dt) {
            velocity.x += acceleration.x * dt;
            velocity.y += acceleration.y * dt;
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
        }
};

class BoundBox {
    public:
        float width;
        float height;
        int topLeft_x;
        int topLeft_y;
        int bottomLeft_x;
        int bottomLeft_y;
        int bottomRight_x;
        int topRight_x;

        BoundBox(float width, float height, const int center_w, const int center_h) : 
        width(width),  
        height(height)
        {
            topLeft_y = center_h - height / 2;
            topLeft_x = center_w - width / 2;
            bottomLeft_x = topLeft_x;
            bottomLeft_y = center_h + height / 2; 
            bottomRight_x = bottomLeft_x + width;
            topRight_x = bottomRight_x;
        }
};

int main() {
    srand(time(NULL));

    const int screenWidth = 1300;
    const int screenHeight = 1000;
    const int Xcenter = screenWidth/2;
    const int Ycenter = screenHeight/2;
    const int FPS = 60;
    const int MAX_OBJECTS = 10;
    const int newObjectFPS = 20;
    const float Bbox_width = 900;
    const float Bbox_height = 900;
    const float Bbox_x = screenWidth/2 - 150;
    const float Bbox_y = screenHeight/2;
    
    InitWindow(screenWidth, screenHeight, "ShittyGame");
    SetTargetFPS(FPS);

    Vector2 acceleration = {0, 0};
    float acceleration_ang = 0.0f; 
    Texture2D arrow = LoadTexture("./resources/arrow.png");
    Texture2D arrow_zero = LoadTexture("./resources/arrow-zero.png");
    
    float arrowX = arrow.width/2;
    float arrowY = arrow.height/2;
    BoundBox bbox(Bbox_width, Bbox_height, Bbox_x, Bbox_y);
    std::vector<Circle> circles;
    int frameCounter = 0;
    bool pour = true;

    while (!WindowShouldClose()) {
        frameCounter++;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            circles.push_back(Circle(10 + randMax(10), 
                                    GetMousePosition(), 
                                    (Vector2){0, 0},
                                    (Color){randMax(90)+(120), randMax(90)+(120),randMax(90)+(120), 255}));
            frameCounter = 0;
        }
    
        if (IsKeyDown(KEY_A)) {acceleration.x--; acceleration_ang = - RAD2DEG * Vector2Angle(acceleration, (Vector2){1.0f, 0.0f});}
        if (IsKeyDown(KEY_D)) {acceleration.x++; acceleration_ang = - RAD2DEG * Vector2Angle(acceleration, (Vector2){1.0f, 0.0f});}
        if (IsKeyDown(KEY_S)) {acceleration.y--; acceleration_ang = - RAD2DEG * Vector2Angle(acceleration, (Vector2){1.0f, 0.0f});}
        if (IsKeyDown(KEY_W)) {acceleration.y++; acceleration_ang = - RAD2DEG * Vector2Angle(acceleration, (Vector2){1.0f, 0.0f});}
        if (IsKeyPressed(KEY_R)) acceleration = (Vector2){0.0f, 0.0f};
        if (IsKeyPressed(KEY_SPACE)) circles.clear();


        float dt = GetFrameTime();
        for(auto &circle_i : circles) {
            circle_i.updatePosition(acceleration, dt);
            //bottom wall
            if ((circle_i.position.y + circle_i.radius) >= bbox.bottomLeft_y) {
                circle_i.position.y = bbox.bottomLeft_y - circle_i.radius;
                circle_i.velocity.y *= -1  * circle_i.restitution;
            }
            //topWall
            else if ((circle_i.position.y - circle_i.radius) <= bbox.topLeft_y) {
                circle_i.position.y = bbox.topLeft_y + circle_i.radius;
                circle_i.velocity.y *= -1  * circle_i.restitution;
            }
            //left wall
            if ((circle_i.position.x - circle_i.radius) <= bbox.bottomLeft_x) {
                circle_i.position.x = bbox.bottomLeft_x + circle_i.radius;
                circle_i.velocity.x *= -1  * circle_i.restitution;
            }
            //rightwall
            else if ((circle_i.position.x + circle_i.radius) >= bbox.bottomRight_x) {
                circle_i.position.x = bbox.bottomRight_x - circle_i.radius;
                circle_i.velocity.x *= -1  * circle_i.restitution;
            }

        }

        BeginDrawing();

            ClearBackground((Color){26, 20, 37, 255});
            for (auto circle_i : circles) {
                circle_i.draw();
            }

            DrawRectangleLines(bbox.topLeft_x, bbox.topLeft_y, bbox.width, bbox.height, (Color){157, 113, 240, 255});

            DrawRectangle(bbox.topRight_x + 25, bbox.topLeft_y , 300, bbox.height, (Color){157, 113, 240, 255});
        
            DrawText("Collision with boundaries only", bbox.topLeft_x, 15, 25, (Color){107, 255, 144, 255});
            DrawText(TextFormat("Objects:  %d", circles.size()),                   bbox.topRight_x + 40,  60, 30, (Color){26, 20, 37, 255});
            DrawText(TextFormat("FPS: %d", GetFPS()),                              bbox.topRight_x + 40,  90, 30, (Color){26, 20, 37, 255});
            DrawText(TextFormat("g: {%.2f, %.2f}", acceleration.x, acceleration.y),bbox.topRight_x + 40, 120, 30, (Color){26, 20, 37, 255});
            DrawText("W: Increase g.y", bbox.topRight_x + 40,  200 - 20, 30, (Color){26, 20, 37, 255});
            DrawText("A: Decrease g.x", bbox.topRight_x + 40,  290 - 20, 30, (Color){26, 20, 37, 255});
            DrawText("S: Decrease g.y", bbox.topRight_x + 40,  230 - 20, 30, (Color){26, 20, 37, 255});
            DrawText("D: Increase g.x", bbox.topRight_x + 40,  260 - 20, 30, (Color){26, 20, 37, 255});
            if (Vector2Equals(acceleration, (Vector2){0})){
                DrawTexturePro( arrow_zero, 
                                (Rectangle){0.0f, 0.0f, arrow.width, arrow.height}, 
                                (Rectangle){bbox.topRight_x + 175, 500, arrow.width*2, arrow.height*2},
                                (Vector2){arrowX*2, arrowY*2}, 
                                0, 
                                WHITE);
            }
            else {
                DrawTexturePro( arrow, 
                                (Rectangle){0.0f, 0.0f, arrow.width, arrow.height}, 
                                (Rectangle){bbox.topRight_x + 175, 500, arrow.width*2, arrow.height*2},
                                (Vector2){arrowX*2, arrowY*2}, 
                                acceleration_ang, 
                                WHITE);
            }
            

        EndDrawing();
    }
    UnloadTexture(arrow); 
    CloseWindow();

    return 0;
}
