#pragma once

class Vector2D
{

private:
    float x;
    float y;

public:
    Vector2D(const float x = 0, const float y = 0);

    float getX() const;
    void setX(const float x);
    float getY() const;
    void setY(const float y);
    Vector2D getVector();
    void setVector(const Vector2D v);

    Vector2D operator+(const Vector2D v);
    Vector2D operator-(const Vector2D v);
    Vector2D operator*(const Vector2D v);
    Vector2D operator/(const Vector2D v);
    bool operator==(const Vector2D v) const;
    bool operator!=(const Vector2D v) const;

    Vector2D operator+(const float n);
    Vector2D operator-(const float n);
    Vector2D operator*(const float n);
    Vector2D operator/(const float n);

    float length() const;
    Vector2D normalize() const;
    Vector2D invert() const;
};