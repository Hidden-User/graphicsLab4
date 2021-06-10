#include "Arrow.h"



Arrow::Arrow()
{
	this->device = nullptr;
	this->context = nullptr;
	this->lines = new LineList();
}


Arrow::~Arrow()
{
	delete this->lines;
	this->context = nullptr;
	this->device = nullptr;
}

void Arrow::rotate(float angleInRad)
{
	Line* line;
	float fb;
	double db;
	_POINT p1;
	_POINT p2;

	for (unsigned t = 0; t < 3u; t++) {
		line = (this->lines)->getLine(t);
		p1 = line->getA();
		p2 = line->getB();
		db = atan2(p1.pos.y, p1.pos.x);
		db += angleInRad;
		fb = sqrtf(p1.pos.x * p1.pos.x + p1.pos.y * p1.pos.y);
		p1.pos.x = cos(db) * fb;
		p1.pos.y = sin(db) * fb;
		db = atan2(p2.pos.y, p2.pos.x);
		db += angleInRad;
		fb = sqrtf(p2.pos.x * p2.pos.x + p2.pos.y * p2.pos.y);
		p2.pos.x = cos(db) * fb;
		p2.pos.y = sin(db) * fb;
		line->resetLine(p1.pos, p2.pos);
	}
}

void Arrow::scale(float v)
{
	Line* line = (this->lines)->getLine(0u);
	float fb = line->getLineLength();
	_POINT p;
	line->setFromAToVec(F2(line->getAngelToB(), line->getLineLength() * v));
	p = line->getB();
	line = (this->lines)->getLine(1u);
	line->setFromAToVec(p.pos, F2(line->getAngelToB(), line->getLineLength() * v));
	line = (this->lines)->getLine(2u);
	line->setFromAToVec(p.pos, F2(line->getAngelToB(), line->getLineLength() * v));
}

void Arrow::move(float x, float y)
{
	Line* line;
	for (unsigned t = 0u; t < (this->lines)->size(); t++) {
		line = (this->lines)->getLine(t);
		line->lineMove(F2(x, y));
	}
}

void Arrow::create(ID3D11Device * device, ID3D11DeviceContext * context)
{
	Line* line;
	_POINT p;
	F4 bf4 = { 0.0f, 1.0f, 0.0f, 1.0f };
	float fb;
	this->device = device;
	this->context = context;
	(this->lines)->setDX(this->device, this->context);

	(this->lines)->setCount(3u);

	line = (this->lines)->getLine(0u);

	line->buildFromPointAndVector({ 0.0f, 0.0f, 0.001f, 1.0f }, F2(DirectX::XM_PIDIV2, 40.0f), { 1.0f, 0.0f, 0.0f, 1.0f });

	p = line->getB();

	fb = line->getAngelToB();

	line = (this->lines)->getLine(1u);

	line->buildFromPointAndVector(p.pos, F2(fb + 135.0f * oneAngleInRad, 10.0f), bf4);

	line = (this->lines)->getLine(2u);

	line->buildFromPointAndVector(p.pos, F2(fb + 225.0f * oneAngleInRad, 10.0f), bf4);

}

void Arrow::draw()
{
	(this->lines)->update();
	(this->lines)->draw();
}
