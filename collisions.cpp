#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>

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
            restitution = 0.8 + randMax(19)/100;
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

        BoundBox(float width, float height, const int screen_w, const int screen_h) : 
        width(width),  
        height(height)
        {
            topLeft_y = screen_h / 2 - height / 2;
            topLeft_x = screen_w / 2 - width / 2;
            bottomLeft_x = topLeft_x;
            bottomLeft_y = screen_h / 2 + height / 2; 
            bottomRight_x = bottomLeft_x + width;
            topRight_x = bottomRight_x;
        }
};

int main() {
    srand(time(NULL));

    const int screenWidth = 1000;
    const int screenHeight = 1000;
    const int Xcenter = screenWidth/2;
    const int Ycenter = screenHeight/2;
    const int FPS = 60;
    const int MAX_OBJECTS = 10;
    const int newObjectFPS = 20;
    const  float Bbox_width = 900;
    const  float Bbox_height = 900;

    Vector2 gravity = {0, 1000};
    BoundBox bbox(Bbox_width, Bbox_height, screenWidth, screenHeight);
    std::vector<Circle> circles;
    int frameCounter = 0;

    InitWindow(screenWidth, screenHeight, "ShittyGame");
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {

        frameCounter++;
        if (frameCounter == 1){
            circles.push_back(Circle(10 + randMax(10), 
                                    GetMousePosition(), 
                                    (Vector2){0, 0},
                                    (Color){randMax(255), randMax(255),randMax(255), 255}));
            frameCounter = 0;
        }
    
        if (IsKeyDown(KEY_A)) gravity.x--;
        if (IsKeyDown(KEY_D)) gravity.x++;
        if (IsKeyDown(KEY_S)) gravity.y--;
        if (IsKeyDown(KEY_W)) gravity.y++;
        if (IsKeyPressed(KEY_R)) gravity = (Vector2){0.0f, 0.0f};
        float dt = GetFrameTime();
        for(auto &circle_i : circles) {
            circle_i.updatePosition(gravity, dt);
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

        // Draw everything
        BeginDrawing();
            ClearBackground(BLACK);
            for (auto circle_i : circles) {
                circle_i.draw();
            }
            DrawRectangleLines(bbox.topLeft_x, bbox.topLeft_y, bbox.width, bbox.height, WHITE);
            DrawRectangle(10, 10, 300, 100, (Color){ 102, 191, 255, 255 });
            DrawText( TextFormat("Objects:  %d", circles.size()), 20, 10, 30, RAYWHITE);
            DrawText( TextFormat("FPS: %d", GetFPS()), 20, 40, 30, RAYWHITE);
            DrawText( TextFormat("g: {%.2f, %.2f}", gravity.x, gravity.y), 20, 70, 30, RAYWHITE);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}