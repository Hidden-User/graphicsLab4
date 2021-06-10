#include "Point.h"


Point::Point()
{
	this->p.pos.x = 0.0f;
	this->p.pos.y = 0.0f;
	this->p.pos.z = 1.0f;
	this->p.pos.w = 1.0f;
}


Point::~Point()
{
	this->p.pos.x = 0.0f;
	this->p.pos.y = 0.0f;
	this->p.pos.z = 0.0f;
	this->p.pos.w = 0.0f;
}

void Point::set(float x, float y)
{
	this->p.pos.x = x;
	this->p.pos.y = y;
}

void Point::mov(float x, float y)
{
	this->p.pos.x += x;
	this->p.pos.y += y;
}

void Point::set(Point a, Point b)
{
	this->p.pos.x = (a.p.pos.x + b.p.pos.x) / 2.0f;
	this->p.pos.y = (a.p.pos.y + b.p.pos.y) / 2.0f;
}

F2 Point::get2DPos()
{
	return { this->p.pos.x, this->p.pos.y };
}
