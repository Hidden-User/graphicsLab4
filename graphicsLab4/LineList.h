#pragma once
#include "PointList.h"



class LineList : GeomObject
{
public:
	LineList();
	~LineList();

	void setDX(ID3D11Device * device, ID3D11DeviceContext * context);

	void setCount(unsigned count);
	Line* getLine(unsigned);
	unsigned size();

	void update();
	void draw();

private:
	Line *lines;
	_POINT* __lines;

};

