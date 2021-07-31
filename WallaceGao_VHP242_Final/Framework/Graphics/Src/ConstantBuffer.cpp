#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace WallG;
using namespace WallG::Graphics;

ConstantBuffer::~ConstantBuffer()
{
    ASSERT(mConstantBuffer == nullptr, "ConstantBuffer -- Constant buffer not released!");
}

void ConstantBuffer::Initialize(uint32_t bufferSize)
{
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = bufferSize;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    auto device = GraphicsSystem::Get()->GetDevice();
    HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer);
    ASSERT(SUCCEEDED(hr), "ConstantBuffer -- Failed to create constant buffer.");
}

void ConstantBuffer::Terminate()
{
    SafeRelease(mConstantBuffer);
}

void ConstantBuffer::Update(const void* data) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}




