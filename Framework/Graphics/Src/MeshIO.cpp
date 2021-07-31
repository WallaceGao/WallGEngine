#include "Precompiled.h"
#include "MeshIO.h"

using namespace WallG::Graphics;
using namespace WallG::Math;

void MeshIO::Write(FILE* file, const SkinnedMesh& mesh)
{
	const uint32_t verticesCount = static_cast<uint32_t>(mesh.vertices.size());
	const uint32_t indicesCount = static_cast<uint32_t>(mesh.indices.size());

	fprintf_s(file, "VertexCount: %d\n", verticesCount);

	for (auto& vertices : mesh.vertices)
	{
		fprintf_s(file, "Position : %f %f %f\n",
			vertices.position.x,
			vertices.position.y,
			vertices.position.z);
		fprintf_s(file, "Normal : %f %f %f\n", 
			vertices.normal.x,
			vertices.normal.y,
			vertices.normal.z);
		fprintf_s(file, "Tangent : %f %f %f\n",
			vertices.tangent.x,
			vertices.tangent.y,
			vertices.tangent.z);
		fprintf_s(file, "UV : %f %f \n",
			vertices.uv.x,
			vertices.uv.y);
		fprintf_s(file, "UV : %d %d %d %d\n", // %d use for int, it print 5 not 5.0 
			vertices.boneIndices[0],
			vertices.boneIndices[1],
			vertices.boneIndices[2],
			vertices.boneIndices[3]);
		fprintf_s(file, "UV : %f %f %f %f\n", // %f use for float it print 5.0 not 5 
			vertices.boneWeights[0],
			vertices.boneWeights[1],
			vertices.boneWeights[2],
			vertices.boneWeights[3]);
	}

	fprintf_s(file, "IndicesCount: %d\n", indicesCount);
	for (size_t i = 0; i < indicesCount; i += 3)
	{
		fprintf_s(file, "%d %d %d\n",
			mesh.indices[i],
			mesh.indices[i + 1],
			mesh.indices[i + 2]);
	}
}

void MeshIO::Read(FILE* file, SkinnedMesh& mesh)
{
	char buffer[255];
	

	uint32_t verticesCount = 0;
	fscanf_s(file, "VertexCount: %d\n", &verticesCount);
	mesh.vertices.reserve(verticesCount);

	for (int i = 0; i < verticesCount; ++i)
	{
		auto& vertices = mesh.vertices.emplace_back();
		fscanf_s(file, "Position : %f %f %f\n",
			&vertices.position.x,
			&vertices.position.y,
			&vertices.position.z);
		fscanf_s(file, "Normal : %f %f %f\n",
			&vertices.normal.x,
			&vertices.normal.y,
			&vertices.normal.z);
		fscanf_s(file, "Tangent : %f %f %f\n",
			&vertices.tangent.x,
			&vertices.tangent.y,
			&vertices.tangent.z);
		fscanf_s(file, "UV : %f %f \n",
			&vertices.uv.x,
			&vertices.uv.y);
		fscanf_s(file, "UV : %d %d %d %d\n",
			&vertices.boneIndices[0],
			&vertices.boneIndices[1],
			&vertices.boneIndices[2],
			&vertices.boneIndices[3]);
		fscanf_s(file, "UV : %f %f %f %f\n",
			&vertices.boneWeights[0],
			&vertices.boneWeights[1],
			&vertices.boneWeights[2],
			&vertices.boneWeights[3]);
	}

	uint32_t indices = 0;
	fscanf_s(file, "IndicesCount: %d\n", &indices);
	mesh.indices.reserve(indices);
	for (size_t i = 0; i < indices; i += 3)
	{
		int i1, i2, i3;
		fscanf_s(file, "%d %d %d\n", &i1, &i2, &i3);
		mesh.indices.push_back(i1);
		mesh.indices.push_back(i2);
		mesh.indices.push_back(i3);
	}
}

void MeshIO::WirteBinary(FILE* file, const SkinnedMesh& mesh)
{
	const uint32_t verticesCount = static_cast<uint32_t>(mesh.vertices.size());
	const uint32_t indicesCount = static_cast<uint32_t>(mesh.indices.size());

	fwrite(mesh.vertices.data(), sizeof(BoneVertex), verticesCount, file);
	fwrite(mesh.indices.data(), sizeof(uint32_t), indicesCount, file);
}

void MeshIO::ReadBinary(FILE* file, SkinnedMesh& mesh)
{

}
