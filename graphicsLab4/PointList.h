#pragma once
#include "Line.h"


class PointList :
	public GeomObject
{
public:
	PointList();
	~PointList();

	void setCount(unsigned count);
	Point* getPoint(unsigned pos);

	void update();
	void draw();


private:
	Point* points;


};

