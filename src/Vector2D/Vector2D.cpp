#include <math.h>

#include "Vector2D.h"

Vector2D::Vector2D(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

float Vector2D::getX() const
{
    return this->x;
}

float Vector2D::getY() const
{
    return this->y;
}

void Vector2D::setX(const float x)
{
    this->x = x;
}

void Vector2D::setY(const float y)
{
    this->y = y;
}

Vector2D Vector2D::getVector()
{
    return Vector2D(this->x, this->y);
}

void Vector2D::setVector(const Vector2D v)
{
    this->x = v.getX();
    this->y = v.getY();
}

float Vector2D::length() const
{
    return std::sqrt(this->x * this->x + this->y * this->y);
}

Vector2D Vector2D::normalize() const
{
    float length = this->length();
    Vector2D vr{this->x, this->y};
    if (length > 1)
    {
        vr.setX(this->x * (1 / length));
        vr.setY(this->y * (1 / length));
    }
    return vr;
}

Vector2D Vector2D::invert() const
{
    return Vector2D(this->x * (-1), this->y * (-1));
}

Vector2D Vector2D::operator+(const Vector2D v)
{
    return Vector2D(this->x + v.getX(), this->y + v.getY());
}

Vector2D Vector2D::operator-(const Vector2D v)
{
    return Vector2D(this->x - v.getX(), this->y - v.getY());
}

Vector2D Vector2D::operator*(const Vector2D v)
{
    return Vector2D(this->x * v.getX(), this->y * v.getY());
}

Vector2D Vector2D::operator/(const Vector2D v)
{
    return Vector2D(this->x / v.getX(), this->y / v.getY());
}

bool Vector2D::operator==(const Vector2D v) const
{
    return (this->x == v.getX()) && (this->y == v.getY());
}

bool Vector2D::operator!=(const Vector2D v) const
{
    return (this->x != v.getX()) || (this->y != v.getY());
}

Vector2D Vector2D::operator+(const float n)
{
    return Vector2D(this->x + n, this->y + n);
}

Vector2D Vector2D::operator-(const float n)
{
    return Vector2D(this->x - n, this->y - n);
}

Vector2D Vector2D::operator*(const float n)
{
    return Vector2D(this->x * n, this->y * n);
}

Vector2D Vector2D::operator/(const float n)
{
    return Vector2D(this->x / n, this->y / n);
}
