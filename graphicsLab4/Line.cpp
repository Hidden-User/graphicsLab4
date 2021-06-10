#include "Line.h"

Line::Line()
{
	this->a = { 0 };
	this->b = { 0 };
}

Line::~Line()
{
	this->a = { 0 };
	this->b = { 0 };
}

void Line::buildFromPointAndVector(F4 posA, F2 vecToB, F4 lCol)
{
	this->a.pos = posA;
	this->a.col = lCol;
	this->b.pos.x = ((float)cos(vecToB.x) * vecToB.y) + this->a.pos.x;
	this->b.pos.y = ((float)sin(vecToB.x) * vecToB.y) + this->a.pos.y;
	this->angleToB = vecToB.x;
	this->angleToA = angleToB + DirectX::XM_PI;
	this->lineLength = vecToB.y;
	this->b.pos.z = posA.z;
	this->b.pos.w = posA.w;
	this->b.col = lCol;
}

void Line::setLine(F4 aPos, F4 bPos, F4 lCol)
{
	this->a.pos = aPos;
	this->b.pos = bPos;
	this->a.col = lCol;
	this->b.col = lCol;
}

void Line::resetLine(F4 aPos, F4 bPos)
{
	this->a.pos = aPos;
	this->b.pos = bPos;
}

void Line::setFromAToVec(F4 pos, F2 vecToB)
{
	this->a.pos.x = pos.x;
	this->a.pos.y = pos.y;
	this->setFromAToVec(vecToB);
}

void Line::setFromAToVec(F2 vecToB)
{
	this->b.pos.x = ((float)cos(vecToB.x) * vecToB.y) + this->a.pos.x;
	this->b.pos.y = ((float)sin(vecToB.x) * vecToB.y) + this->a.pos.y;
	this->angleToB = vecToB.x;
	this->angleToA = this->angleToB + DirectX::XM_PI;
	this->lineLength = vecToB.y;
}

void Line::setFromBToVec(F4 pos, F2 vecToB)
{
	this->a.pos.x = pos.x;
	this->a.pos.y = pos.y;
	this->setFromBToVec(vecToB);
}

void Line::setFromBToVec(F2 vecToA)
{
	this->a.pos.x = ((float)cos(vecToA.x) * vecToA.y) + this->b.pos.x;
	this->a.pos.y = ((float)sin(vecToA.x) * vecToA.y) + this->b.pos.y;
	this->angleToA = vecToA.x;
	this->angleToB = this->angleToA - DirectX::XM_PI;
	this->lineLength = vecToA.y;
}

void Line::lineMove(F2 vMove) {
	this->a.pos.x += vMove.x;
	this->a.pos.y += vMove.y;
	this->b.pos.x += vMove.x;
	this->b.pos.y += vMove.y;
}

void Line::selfCopy(_POINT * points)
{
	points[0] = this->a;
	points[1] = this->b;
}

_POINT Line::getA()
{
	return this->a;
}

_POINT Line::getB()
{
	return this->b;
}


float Line::getAngleToA() {
	return this->angleToA;
}

float Line::getAngelToB() {
	return this->angleToB;
}

float Line::getLineLength() {
	return this->lineLength;
}
