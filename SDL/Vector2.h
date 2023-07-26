#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
public:
    int x;
    int y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(const int x, const int y) : x(x), y(y) {}

    // Add member functions for vector operations
    Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2& other) const {
        return {x - other.x, y - other.y};
    }

    Vector2 operator*(const int scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector2 operator/(const int scalar) const {
        return {x / scalar, y / scalar};
    }
};

#endif 