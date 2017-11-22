#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(double x, double y);
	~Vector2D();
	Vector2D& operator+=(Vector2D &v2);
	Vector2D& operator-=(const Vector2D &v2);
	Vector2D& operator+(const Vector2D &v2);
	Vector2D& operator-(const Vector2D &v2);
	Vector2D& operator*(const Vector2D &v2);
	Vector2D& operator/(const Vector2D &v2);
	double x;
	double y;
};

