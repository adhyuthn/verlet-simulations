#include <raylib.h>

class CircleEuler {
    public:
        float radius;
        Vector2 position;
        Vector2 velocity;
        Color color;
        float restitution;

        CircleEuler(float radius, Vector2 x, Vector2 v, Color color, float restitution):
            radius(radius),
            position(x),
            velocity(v),
            color(color),
            restitution(restitution)
        {}

        void draw() {-
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
