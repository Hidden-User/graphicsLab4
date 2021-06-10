#pragma once
#include "LineList.h"



class Arrow
{
public:
	Arrow();
	~Arrow();

	void rotate(float angleInRad);
	void scale(float v);
	void move(float x, float y);

	void create(ID3D11Device* device, ID3D11DeviceContext* context);
	void draw();

private:

	LineList* lines;

	ID3D11DeviceContext* context;
	ID3D11Device* device;

};

