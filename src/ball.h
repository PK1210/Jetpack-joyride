#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void tickUp();
    void move(int direction);
    double speed;
private:
    VAO *object;
    double speed_x;
};

#endif // BALL_H
