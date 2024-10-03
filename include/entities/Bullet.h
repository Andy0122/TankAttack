#ifndef BULLET_H
#define BULLET_H

#include "Position.h"


struct Direction {
    float x;
    float y;
};

class Bullet {
public:
    Bullet(Position origin, Position target);

    ~Bullet() = default;

    bool move();
    [[nodiscard]] bool checkCollision(const Position& other) const;
    [[nodiscard]] Position getPosition() const;

private:
    Position position;
    Position target;
    Direction direction;
    float speed = 1.0;
    float bounceAngle = 0.0;
    int damage;

    [[nodiscard]] float calculateDistance() const;
    // void handleBounce();
};



#endif //BULLET_H
