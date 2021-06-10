#pragma once

#include "Windows.h"
#include "d3d11.h"
#include "DirectXMath.h"


class GeomObject
{
public:
	GeomObject() {
		this->indexes = nullptr;
		this->pointBuff = nullptr;
		this->indexBuff = nullptr;
		this->context = nullptr;
		this->device = nullptr;
	}
	virtual ~GeomObject() {
		if (this->indexes != nullptr) {
			delete[] this->indexes;
		}
		if (this->pointBuff != nullptr) {
			(this->pointBuff)->Release();
		}
		if (this->indexBuff != nullptr) {
			(this->indexBuff)->Release();
		}
		this->context = nullptr;
		this->device = nullptr;
	}

	virtual void setDX(ID3D11Device* device, ID3D11DeviceContext* context) { this->device = device;	this->context = context; }

	virtual void setCount(unsigned count) = 0;
	virtual unsigned size() { return this->count; }

	virtual void update() = 0;
	virtual void draw() = 0;

protected:
	unsigned *indexes;
	unsigned count;

	ID3D11Buffer* pointBuff;
	ID3D11Buffer* indexBuff;
	ID3D11DeviceContext* context;
	ID3D11Device* device;

	HRESULT initIndexes(unsigned pointInOneObject) {
		if (this->device == nullptr) {
			throw 0xFFFF;
		}
		this->indexes = new unsigned[pointInOneObject * this->count];

		for (unsigned t = 0; t < pointInOneObject * this->count; t++) {
			this->indexes[t] = t;
		}

		D3D11_SUBRESOURCE_DATA indexSR;

		indexSR.pSysMem = &(this->indexes[0]);
		indexSR.SysMemPitch = 0;
		indexSR.SysMemSlicePitch = 0;

		D3D11_BUFFER_DESC* ibd = new D3D11_BUFFER_DESC;
		ibd->Usage = D3D11_USAGE_DEFAULT;
		ibd->ByteWidth = sizeof(unsigned) * this->count * pointInOneObject;
		ibd->BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd->CPUAccessFlags = 0;
		ibd->MiscFlags = 0;
		ibd->StructureByteStride = 0;

		return (this->device)->CreateBuffer(ibd, &indexSR, &(this->indexBuff));

	}

};
