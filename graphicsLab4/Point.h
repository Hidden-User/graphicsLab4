#pragma once
#include "GeomObject.h"


const float oneAngleInRad = 0.0174533;

struct F2
{
	F2() {
		this->x = 0.0f;
		this->y = 0.0f;
	}
	F2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	float x;
	float y;
};

struct F3
{
	float x;
	float y;
	float z;
};

struct F4
{
	float x;
	float y;
	float z;
	float w;
};

struct _POINT
{
	F4 pos;
	F4 col;
};

struct cBuffer
{
	DirectX::XMMATRIX projection;
};

class Point
{
public:
	Point();
	~Point();

	Point& operator=(const F2& f) {
		this->p.pos.x = f.x;
		this->p.pos.y = f.y;
		return *this;
	}

	void set(float x, float y);
	void mov(float x, float y);
	void set(Point a, Point b);

	F2 get2DPos();

private:

	_POINT p;

};

