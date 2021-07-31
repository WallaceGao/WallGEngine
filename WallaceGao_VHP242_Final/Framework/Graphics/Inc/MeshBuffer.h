#pragma once

#include "MeshTypes.h"
namespace WallG::Graphics
{
	class MashBuffer
	{
	public:
		MashBuffer() = default;
		~MashBuffer();

		MashBuffer(const MashBuffer&) = delete;
		MashBuffer& operator=(const MashBuffer&) = delete;

		template<class MeshType>
		void Initialize(const MeshType& mesh)
		{
			Initialize(
				mesh.vertices.data(),
				static_cast<uint32_t>(sizeof(MeshType::VertexType)),
				static_cast<uint32_t>(mesh.vertices.size()),
				mesh.indices.data(),
				static_cast<uint32_t>(mesh.indices.size()));
		}

		void Initialize(const void* vertexData, uint32_t VertexSize, uint32_t VertexCount);
		void Initialize(const void* vertexData, uint32_t VertexSize, uint32_t VertexCount, const uint32_t* indexData, uint16_t indexCount);
		void Terminate();

		void Render() const;

	private:
		ID3D11Buffer* mMashBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;
		uint32_t mVertexSize = 0;
		uint32_t mVertexCount = 0;
		uint32_t mIndexCount = 0;
	};
}
