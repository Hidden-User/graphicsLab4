#include "LineList.h"

LineList::LineList()
{
	this->lines = NULL;
	this->__lines = NULL;
}

LineList::~LineList()
{
	if (this->lines != NULL) {
		delete[] this->lines;
	}
	if (this->__lines != NULL) {
		delete[] this->__lines;
	}
}

void LineList::setDX(ID3D11Device* device, ID3D11DeviceContext* context)
{ 
	this->device = device;
	this->context = context;
}

void LineList::setCount(unsigned count)
{
	if (this->device == NULL || this->context == NULL) {
		return;
	}

	HRESULT hr = NULL;

	this->lines = new Line[count];
	this->__lines = new _POINT[count * 2];
	this->count = count;
	hr = this->initIndexes(2u);

	if (FAILED(hr)) {
		MessageBox(NULL, "error create buff", "err", MB_ICONSTOP | MB_OK);
	}

	Line ll = Line();

	for (unsigned t = 0; t < this->count; t++) {
		this->lines[t] = ll;
	}

	D3D11_SUBRESOURCE_DATA _LinesSR;

	_LinesSR.pSysMem = this->lines;
	_LinesSR.SysMemPitch = 0;
	_LinesSR.SysMemSlicePitch = 0;

	D3D11_BUFFER_DESC pbd;
	pbd.Usage = D3D11_USAGE_DYNAMIC;
	pbd.ByteWidth = sizeof(Line) * this->count;
	pbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	pbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pbd.MiscFlags = 0;
	pbd.StructureByteStride = 0;

	hr = (this->device)->CreateBuffer(&pbd, &_LinesSR, &this->pointBuff);

	if (FAILED(hr)) {
		MessageBox(NULL, "error create buff", "err", MB_ICONSTOP | MB_OK);
	}

}

Line * LineList::getLine(unsigned pos)
{
	if (pos >= this->count) {
		return nullptr;
	}
	return &(this->lines[pos]);
}

unsigned LineList::size()
{
	return this->count;
}

void LineList::update()
{
	D3D11_MAPPED_SUBRESOURCE mpsr = { 0 };

	for (unsigned t = 0; t < this->count; t++) {
		(this->lines[t]).selfCopy(&(this->__lines[t * 2]));
		//this->__lines[t * 2] = (this->lines[t]).getA();
		//this->__lines[t * 2 + 1] = (this->lines[t]).getB();
	}

	(this->context)->Map(this->pointBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mpsr);

	memcpy(mpsr.pData, this->__lines, this->count * sizeof(_POINT) * 2);

	(this->context)->Unmap(this->pointBuff, 0);
}

void LineList::draw()
{
	UINT stride = sizeof(_POINT);
	UINT offset = 0;

	(this->context)->IASetVertexBuffers(0, 1, &(this->pointBuff), &stride, &offset);
	(this->context)->IASetIndexBuffer(this->indexBuff, DXGI_FORMAT_R32_UINT, 0);

	(this->context)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	(this->context)->DrawIndexed(this->count * 2, 0, 0);

}
