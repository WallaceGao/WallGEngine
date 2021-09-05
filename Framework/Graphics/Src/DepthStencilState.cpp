#include "Precompiled.h"
#include "DepthStencilState.h"

#include "GraphicsSystem.h"

using namespace WallG::Graphics;

void DepthStencilState::ClearState()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(nullptr, 0);
}

DepthStencilState::~DepthStencilState()
{
	ASSERT(mDepthStencilState == nullptr, "DepthStencilState -- Depth stencil state not released!");
}

// From VGP240:
//==============
// 
//	void DrawPixel(float x, float y, float z, Color c)
//	{
//		// Check if this pixel has a closer z
//		if (!depthEanble || DepthBuffer::Get()->CheckDepth(x, y, z))
//		{
//			if (depthWrite)
//				DepthBuffer::Get()->SetDepth(x, y, z);
//			X::DrawPixel(x, y, c);
//		}
//	 }
// 

void DepthStencilState::Initialize(bool depthEnable, bool depthWrite)
{
	D3D11_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = depthEnable ? TRUE : FALSE;
	desc.DepthWriteMask = depthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateDepthStencilState(&desc, &mDepthStencilState);
	ASSERT(SUCCEEDED(hr), "DepthStencilState -- Failed to create depth stencil state.");
}

void DepthStencilState::Terminate()
{
	SafeRelease(mDepthStencilState);
}

void DepthStencilState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(mDepthStencilState, 0);
}