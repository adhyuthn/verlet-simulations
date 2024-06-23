#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "objects.hpp"
#include "colors.hpp"

inline int randMax(int a) {
    return rand() % a;
}

inline int randDir() {
    return rand() % 2 == 0 ? 1 : -1;
}

void solveCollisions(std::vector<CircleEuler> &circles) {
    for (auto &circle_i : circles) {
        for (auto &circle_j : circles) {
            if (&circle_i != &circle_j) {
                float sum_radius = circle_i.radius + circle_j.radius;
                Vector2 axis = Vector2Subtract(circle_i.position, circle_j.position);
                float dist = Vector2Length(axis);
                if (dist < sum_radius) {
                    float overlap = (sum_radius - dist)/2;
                    Vector2 normal = Vector2Normalize(axis);
                    circle_i.position = Vector2Add(circle_i.position, Vector2Scale(axis, overlap));
                }
            } 
        }
    }
}

int main() {
    
    const int screenWidth = 1300;
    const int screenHeight = 1000;
    const int Xcenter = screenWidth/2;
    const int Ycenter = screenHeight/2;
    const int FPS = 60;
    const int MAX_OBJECTS = 10;
    const int newObjectFPS = 20;
    const int sub_step = 1;
    const float Bbox_width = 900;
    const float Bbox_height = 900;
    const float Bbox_x = screenWidth/2 - 150;
    const float Bbox_y = screenHeight/2;
    
    InitWindow(screenWidth, screenHeight, "ShittyGame");
    SetTargetFPS(FPS);

    float acceleration_ang = 0.0f; 
    int frameCounter = 0;
    bool pour = true;
    Vector2 acceleration = {0, 0};
    Texture2D arrow = LoadTexture("../resources/arrow.png");
    Texture2D arrow_zero = LoadTexture("../resources/arrow-zero.png");
    Vector2 mousePos = (Vector2){0.0f, 0.0f};
    
    BoundBox bbox(Bbox_width, Bbox_height, Bbox_x, Bbox_y);
    std::vector<CircleEuler> circles;

    while (!WindowShouldClose()) {
        frameCounter++;
        mousePos = GetMousePosition();
        
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            for (int i = 0; i <= 10; i++){
                circles.push_back(CircleEuler(5 + randMax(10), 
                                        mousePos, 
                                        (Vector2){ randDir()  * randMax(400),  randDir() * randMax(400)},
                                        (Color){randMax(90)+(120), randMax(90)+(120),randMax(90)+(120), 255},
                                        0.8 + randMax(2)/10.0));
                frameCounter = 0;
            }
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
            // for (int i = 0; i <= sub_step; i++) solveCollisions(circles);
            for (auto circle_i : circles) {circle_i.draw();}

            DrawRectangleLines(bbox.topLeft_x, bbox.topLeft_y, bbox.width, bbox.height, (Color){157, 113, 240, 255});
            
            DrawRectangleLines(bbox.topRight_x + 25, bbox.topLeft_y , 300, bbox.height, MOCHA_LIGHT);
            DrawText("Collision with boundaries only", bbox.topLeft_x, 15, 25, MOCHA_GREEN);
            DrawText(TextFormat("Objects:  %d", circles.size()),                   bbox.topRight_x + 40,  60, 30, MOCHA_GREEN);
            DrawText(TextFormat("FPS: %d", GetFPS()),                              bbox.topRight_x + 40,  90, 30, MOCHA_GREEN);
            DrawText(TextFormat("g: {%.2f, %.2f}", acceleration.x, acceleration.y),bbox.topRight_x + 40, 120, 30, MOCHA_GREEN);
            DrawText("W: Increase g.y", bbox.topRight_x + 40,  200 - 20, 30, MOCHA_LIGHT);
            DrawText("S: Decrease g.y", bbox.topRight_x + 40,  230 - 20, 30, MOCHA_LIGHT);
            DrawText("D: Increase g.x", bbox.topRight_x + 40,  260 - 20, 30, MOCHA_LIGHT);
            DrawText("A: Decrease g.x", bbox.topRight_x + 40,  290 - 20, 30, MOCHA_LIGHT);
            DrawText("R: Reset g",      bbox.topRight_x + 40,  320 - 20, 30, MOCHA_LIGHT);
            DrawText("Space: Snap",         bbox.topRight_x + 40,  350 - 20, 30, MOCHA_LIGHT);
            DrawText(TextFormat("x: %0.2f y: %0.2f ", mousePos.x, mousePos.y), bbox.bottomRight_x + 40, bbox.bottomLeft_y - 40, 30, MOCHA_GREEN);
            if (Vector2Equals(acceleration, (Vector2){0})){
                DrawTexturePro( arrow_zero, 
                                (Rectangle){0.0f, 0.0f, arrow.width, arrow.height}, 
                                (Rectangle){bbox.topRight_x + 175, 600, arrow.width/4, arrow.height/4},
                                (Vector2){arrow.width/8, arrow.width/8}, 
                                0, 
                                WHITE );
            }
            else {
                DrawTexturePro( arrow, 
                                (Rectangle){0.0f, 0.0f, arrow.width, arrow.height}, 
                                (Rectangle){bbox.topRight_x + 175, 600, arrow.width/4, arrow.height/4},
                                (Vector2){arrow.width/8, arrow.width/8}, 
                                acceleration_ang, 
                                WHITE );
            }
            EndDrawing();
            
    }
    UnloadTexture(arrow); 
    CloseWindow();
    return 0;
}