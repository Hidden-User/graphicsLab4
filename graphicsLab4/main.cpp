#include"Arrow.h"
//#include "pointVector.h"
#include "d3dcompiler.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;

HWND hWnd;
HINSTANCE hInstance;

const wchar_t* windowName = L"Lab2";
const wchar_t *windowClassName = L"Lab2";

const char vertexShader[] = "cbuffer CB:register(b0){matrix proj;}struct POINT{float4 pos:SV_POSITION;float4 color:COLOR0;};POINT VS(float4 position:POSITION,float4 color:COLOR){POINT _out=(POINT)0;_out.pos=mul(position,proj);_out.color=color;return _out;}";
const char pixelShader[] = "struct POINT{float4 pos:SV_POSITION;float4 color:COLOR0;};float4 PS(POINT input):SV_Target{return input.color;}";

IDXGISwapChain* swapChain;
ID3D11Device* device;
ID3D11DeviceContext* context;
ID3D11RenderTargetView* targetView;
ID3D11Texture2D* depthStencil;
ID3D11DepthStencilState* DSS_zOn;
ID3D11DepthStencilState* DSS_zOff;
ID3D11DepthStencilView* depthStencilView;
ID3D11RenderTargetView* renderTargetView;
ID3D11InputLayout* layout;
ID3D11Buffer* cBuff;

ID3D11VertexShader* vs;
ID3D11PixelShader* ps;

F4 backGrColor = { 0.79f, 0.79f, 0.79f, 1.0 };
float BC[] = { 1.0f, 1.0f, 1.0f, 1.0f };
cBuffer constB;
F4 pointsColor = { 1.0f, 0.0f, 0.0f, 1.0f };
//F4 arrow = { 0.0f, 0.0f, 0.0f, 0.0f };
F2 viewArrow = { 0.0f, 0.0f };
unsigned arrowIndex;
bool prepareToDie = false;
Arrow* arrow = NULL;
PointList* pl = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void createWindow() {
	hInstance = GetModuleHandle(NULL);
	HCURSOR hCur = LoadCursor(hInstance, IDC_ARROW);

	LOGBRUSH logBr = { BS_SOLID, RGB(0,0,0), 0 };
	HBRUSH hbrBackGr = CreateBrushIndirect(&logBr);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = hCur;
	wc.hbrBackground = hbrBackGr;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)windowClassName;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	hWnd = CreateWindow(
		(LPCSTR)windowClassName,
		(LPCSTR)windowName,
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		1280,
		800,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd) {
		throw 0;
	}

	ShowWindow(hWnd, SW_SHOW);

}

void startDX() {

	HRESULT hr = NULL;

	RECT rect = { 0 };
	GetClientRect(hWnd, &rect);
	DWORD wsX = rect.right - rect.left;
	DWORD wsY = rect.bottom - rect.top;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = wsX;
	scd.BufferDesc.Height = wsY;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		NULL,
		&context
	);

	ID3D11Texture2D *backBuff = NULL;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuff);
	device->CreateRenderTargetView(backBuff, NULL, &targetView);

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = wsX;
	descDepth.Height = wsY;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device->CreateTexture2D(&descDepth, NULL, &depthStencil);

	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = true;
	dsd.StencilReadMask = 0xFF;
	dsd.StencilWriteMask = 0xFF;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	device->CreateDepthStencilState(&dsd, &DSS_zOn);
	dsd.DepthEnable = false;
	device->CreateDepthStencilState(&dsd, &DSS_zOff);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;
	device->CreateDepthStencilView(depthStencil, &descDSV, &depthStencilView);

	context->OMSetRenderTargets(1, &targetView, depthStencilView);

	D3D11_VIEWPORT vp;
	vp.Width = (float)wsX;
	vp.Height = (float)wsY;
	vp.MinDepth = 0.0001f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	constB.projection = DirectX::XMMatrixOrthographicLH(vp.Width, vp.Height, 0.0f, 1.0f);


	D3D11_INPUT_ELEMENT_DESC inElDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3DBlob* lBlob = NULL;
	ID3DBlob* errBlob = NULL;

	D3DCompile(vertexShader, sizeof(vertexShader), "vertexShader", NULL, NULL, "VS", "vs_4_1", NULL, NULL, &lBlob, &errBlob);

	if (errBlob != 0) {
		MessageBox(NULL, (LPCSTR)errBlob->GetBufferPointer(), "ERR", MB_ICONSTOP | MB_OK);
		errBlob->Release();
		errBlob = NULL;
	}

	device->CreateVertexShader(lBlob->GetBufferPointer(), lBlob->GetBufferSize(), NULL, &vs);

	device->CreateInputLayout(inElDesc, 2, lBlob->GetBufferPointer(), lBlob->GetBufferSize(), &layout);

	context->IASetInputLayout(layout);

	D3DCompile(pixelShader, sizeof(pixelShader), "pixelShader", NULL, NULL, "PS", "ps_4_1", NULL, NULL, &lBlob, &errBlob);

	if (errBlob != 0) {
		MessageBox(NULL, (LPCSTR)errBlob->GetBufferPointer(), "ERR", MB_ICONSTOP | MB_OK);
		errBlob->Release();
		errBlob = NULL;
	}

	device->CreatePixelShader(lBlob->GetBufferPointer(), lBlob->GetBufferSize(), NULL, &ps);

	lBlob->Release();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(cBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bd, NULL, &cBuff);

	if (FAILED(hr)) {
		MessageBox(NULL, "error create const buff", "err", MB_ICONSTOP | MB_OK);
	}
}

void stopDX() {
	if (cBuff != NULL) {
		cBuff->Release();
	}
	if (ps != NULL) {
		ps->Release();
	}
	if (layout != NULL) {
		layout->Release();
	}
	if (vs != NULL) {
		vs->Release();
	}
	if (depthStencilView != NULL) {
		depthStencilView->Release();
	}
	if (DSS_zOff != NULL) {
		DSS_zOff->Release();
	}
	if (DSS_zOn != NULL) {
		DSS_zOn->Release();
	}
	if (depthStencil != NULL) {
		depthStencil->Release();
	}
	if (targetView != NULL) {
		targetView->Release();
	}
	if (swapChain != NULL) {
		swapChain->Release();
	}
	if (device != NULL) {
		device->Release();
	}
}

int main() {

	bool active = true;
	MSG msg;
	float _backGr[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	cBuffer lCBuffer;

	Point* p;

	createWindow();
	startDX();

	arrow = new Arrow();
	arrow->create(device, context);

	pl = new PointList();
	pl->setDX(device, context);
	pl->setCount(5u);

	for (unsigned t = 0; t < 5u; t++) {
		p = pl->getPoint(t);
		p->set((rand() % 1280) - 640.0f, (rand() % 800) - 400.0f);
	}

	while (active)
	{
		if (prepareToDie) {
			break;
		}
		while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}

		context->OMSetDepthStencilState(DSS_zOff, 1);
		context->ClearRenderTargetView(targetView, _backGr);
		context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, (UINT8)0);

		lCBuffer.projection = DirectX::XMMatrixTranspose(constB.projection);

		context->UpdateSubresource(cBuff, 0, NULL, &lCBuffer, 0, 0);
		context->VSSetConstantBuffers(0, 1, &cBuff);

		context->VSSetShader(vs, NULL, NULL);
		context->PSSetShader(ps, NULL, NULL);

		arrow->draw();
		pl->update();
		pl->draw();

		swapChain->Present(1, 0);
	}

	delete arrow;
	stopDX();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT res = NULL;
	switch (message)
	{
	default:
		res = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		default:
			break;
		case 0x53:// S
			arrow->move(0.0f, -1.0f);
			break;
		case 0x57:// W
			arrow->move(0.0f, 1.0f);
			break;
		case 0x41:// A
			arrow->move(-1.0f, 0.0f);
			break;
		case 0x44:// D
			arrow->move(1.0f, 0.0f);
			break;
		case 0x51:// Q
			arrow->rotate(oneAngleInRad);
			break;
		case 0x45:// E
			arrow->rotate(oneAngleInRad * -1.0f);
			break;
		case 0x52:// R
			arrow->scale(1.1f);
			break;
		case 0x46:// F
			arrow->scale(0.9f);
			break;
		}
		break;
		res = NULL;
	case WM_DESTROY:
		prepareToDie = true;
		PostQuitMessage(0);
		break;
	}
	return res;
}
