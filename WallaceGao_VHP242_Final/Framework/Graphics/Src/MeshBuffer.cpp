#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"


using namespace WallG::Graphics;

MashBuffer::~MashBuffer()
{
    ASSERT(mMashBuffer == nullptr, "MashBuffer == Terminate must be called before destruction");
    ASSERT(mIndexBuffer == nullptr, "IndexBuffer == Terminate must be called before destruction");
}

void MashBuffer::Initialize(const void* vertexData, uint32_t VertexSize, uint32_t VertexCount)
{
    Initialize(vertexData, VertexSize, VertexCount, nullptr, 0);
}

void MashBuffer::Initialize(const void* vertexData, uint32_t VertexSize, uint32_t VertexCount, const uint32_t* indexData, uint16_t indexCount)
{
    mVertexSize = VertexSize;
    mVertexCount = VertexCount;
    mIndexCount = indexCount;

    auto device = GraphicsSystem::Get()->GetDevice();
    // -------------------------------------------
    // Create vertex buffer
    D3D11_BUFFER_DESC bufferDesc{};

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    bufferDesc.ByteWidth = (UINT)(VertexCount * VertexSize);

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = vertexData;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mMashBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

    //Creat index buffer if data is provided
    if (indexData && indexCount > 0)
    {
        bufferDesc.ByteWidth = (UINT)(indexCount * sizeof(uint32_t));
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        initData.pSysMem = indexData;

        hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
        ASSERT(SUCCEEDED(hr), "Failed to create index buffer.");
    }
}

void MashBuffer::Terminate()
{
    SafeRelease(mMashBuffer);
    SafeRelease(mIndexBuffer);
}

void MashBuffer::Render() const
{
    auto context = GraphicsSystem::Get()->GetContext();

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT stride = (UINT)mVertexSize;
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mMashBuffer, &stride, &offset);

    //Check if we have index data
    if (mIndexBuffer != nullptr)
    {
        context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        context->DrawIndexed(mIndexCount, 0, 0);
    }
    else
    {
        context->Draw((UINT)mVertexCount, 0);
    }
    context->Draw((UINT)mVertexCount, 0);
}
