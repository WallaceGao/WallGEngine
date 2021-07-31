#pragma once

#include "VertexTypes.h"

namespace WallG::Graphics
{
	template <class VertexT>
	struct MeshBase
	{
		using VertexType = VertexT;
		std::vector<VertexT> vertices; // points of the mesh
		std::vector<uint32_t> indices; // the gpu which vertices to use to draw the triangle
	};

	using MeshPC = MeshBase<VertexPC>;
	using MeshPX = MeshBase<VertexPX>;
	using MeshPN = MeshBase<VertexPN>;
	using Mesh = MeshBase<Vertex>;
	using SkinnedMesh = MeshBase<BoneVertex>;
}