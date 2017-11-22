#include "stdafx.h"
#include "Vector2D.h"


Vector2D::Vector2D(){
}

Vector2D::Vector2D(double x, double y) {
	this->x = x;
	this->y = y;
}

Vector2D::~Vector2D(){
}

Vector2D& Vector2D::operator+=(Vector2D &v2) {
	double xLokal= this->x + v2.x;
	double yLokal=this->y + v2.y;
	/*if (xLokal > 1920) {
		xLokal -= 1920;
	}
	else if (xLokal < 0) {
		xLokal += 1920;
	}
	if (yLokal > 1080) {
		yLokal -= 1080;
	}
	else if (yLokal < 0) {
		yLokal += 1080;
	}*/
	if (xLokal > 200) {
		xLokal -= 200;
	}
	else if (xLokal < 0) {
		xLokal += 200;
	}
	if (yLokal > 200) {
		yLokal -= 200;
	}
	else if (yLokal < 0) {
		yLokal += 200;
	}
	this->x = xLokal;
	this->y = yLokal;
	return *this;
};

Vector2D& Vector2D::operator-=(const Vector2D &v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	return *this;
};

Vector2D& Vector2D::operator+(const Vector2D &v2) {
	return Vector2D(this->x + v2.x, this->y + v2.y);
}

Vector2D& Vector2D::operator-(const Vector2D &v2) {
	return Vector2D(this->x - v2.x, this->y - v2.y);
}

Vector2D& Vector2D::operator*(const Vector2D &v2) {
	return Vector2D(this->x * v2.x, this->y * v2.y);
}

Vector2D& Vector2D::operator/(const Vector2D &v2) {
	return Vector2D(this->x / v2.x, this->y / v2.y);
}