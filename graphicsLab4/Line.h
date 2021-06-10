#pragma once
#include "Point.h"


class Line
{
public:

	Line();
	~Line();

	void buildFromPointAndVector(F4, F2, F4 = { 0 });
	void setLine(F4, F4, F4 = { 0 });
	void resetLine(F4, F4);
	void setFromAToVec(F2);
	void setFromBToVec(F2);
	void setFromAToVec(F4, F2);
	void setFromBToVec(F4, F2);
	void lineMove(F2);

	void selfCopy(_POINT* points);

	_POINT getA();
	_POINT getB();

	float getAngleToA();
	float getAngelToB();
	float getLineLength();

private:

	_POINT a;
	_POINT b;

	float angleToB;
	float angleToA;
	float lineLength;

};

