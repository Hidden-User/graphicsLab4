#include "PointList.h"


PointList::PointList()
{
}


PointList::~PointList()
{
}

void PointList::setCount(unsigned count)
{
	HRESULT hr;

	this->points = new Point[count];
	this->count = count;
	hr = this->initIndexes(1u);

	if (FAILED(hr)) {
		MessageBox(NULL, "error create index buff", "err", MB_ICONSTOP | MB_OK);
	}

	D3D11_SUBRESOURCE_DATA pointSR;
	pointSR.pSysMem = this->points;
	pointSR.SysMemPitch = 0;
	pointSR.SysMemSlicePitch = 0;

	D3D11_BUFFER_DESC* bd = new D3D11_BUFFER_DESC;
	bd->Usage = D3D11_USAGE_DYNAMIC;
	bd->ByteWidth = sizeof(Point) * this->count;
	bd->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd->MiscFlags = 0;
	bd->StructureByteStride = 0;

	hr = (this->device)->CreateBuffer(bd, &pointSR, &(this->pointBuff));

	if (FAILED(hr)) {
		MessageBox(NULL, "error create point buff", "err", MB_ICONSTOP | MB_OK);
	}

}

Point * PointList::getPoint(unsigned pos)
{
	if (pos >= this->count) {
		throw 0xFFFF;
	}
	return &(this->points[pos]);
}

void PointList::update()
{
	D3D11_MAPPED_SUBRESOURCE mpsr = { 0 };

	(this->context)->Map(this->pointBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mpsr);

	memcpy_s(mpsr.pData, this->count * sizeof(Point), this->points, this->count * sizeof(Point));

	(this->context)->Unmap(this->pointBuff, 0);

}

void PointList::draw() {
	UINT stride = sizeof(_POINT);
	UINT offset = 0;

	(this->context)->IASetVertexBuffers(0, 1, &(this->pointBuff), &stride, &offset);
	(this->context)->IASetIndexBuffer(this->indexBuff, DXGI_FORMAT_R32_UINT, 0);

	(this->context)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	(this->context)->DrawIndexed(this->count, 0, 0);
}
