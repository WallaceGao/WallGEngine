#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"


using namespace WallG::Graphics;

MeshBuffer::~MeshBuffer()
{
	ASSERT(mVertexBuffer == nullptr, "MeshBuffer -- Terminate must be called before destruction.");
	ASSERT(mIndexBuffer == nullptr, "MeshBuffer -- Terminate must be called before destruction.");
}

void MeshBuffer::Initialize(const void* vertexData, uint32_t vertexSize, uint32_t vertexCount, bool dynamic)
{
	Initialize(vertexData, vertexSize, vertexCount, nullptr, 0, dynamic);
}

void MeshBuffer::Initialize(const void* vertexData, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indexData, uint32_t indexCount, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;
	mIndexCount = indexCount;

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = (UINT)(vertexCount * vertexSize);

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertexData;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&bufferDesc, vertexData ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "MeshBuffer -- Failed to create vertex buffer.");

	// Create index buffer if data is provided
	if (indexData && indexCount > 0)
	{
		bufferDesc.ByteWidth = (UINT)(indexCount * sizeof(uint32_t));
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = indexData;

		hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
		ASSERT(SUCCEEDED(hr), "MeshBuffer -- Failed to create index buffer.");
	}
}

void MeshBuffer::Terminate()
{
	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);
}

void MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::Points:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case Topology::Lines:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Topology::Triangles:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}
}

void MeshBuffer::Update(const void* vertices, uint32_t numVertices)
{
	mVertexCount = numVertices;

	auto context = GraphicsSystem::Get()->GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertices, numVertices * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}

void MeshBuffer::Render() const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->IASetPrimitiveTopology(mTopology);

	UINT stride = (UINT)mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// Check if we have index data
	if (mIndexBuffer != nullptr)
	{
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		context->Draw((UINT)mVertexCount, 0);
	}
}
