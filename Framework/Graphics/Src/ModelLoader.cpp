#include "Precompiled.h"
#include "ModelLoader.h"
#include "MaterialIO.h"
#include "MeshIO.h"
#include "SkeletonIO.h"
#include "AnimationIO.h"

using namespace WallG;
using namespace WallG::Graphics;

void ModelLoader::LoadObjPX(std::filesystem::path fileName, float scale, MeshPX& mesh)
{
	std::vector<uint32_t> positionIndices, uvIndices;
	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector2> uvs;

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "[ModelLoader] Failed to open file %s.", fileName.u8string().c_str());

	while (true)
	{
		char buffer[128];
		int res = fscanf_s(file, "%s", buffer, (unsigned int)std::size(buffer));
		if (res == EOF)
			break;

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back({ uv.x, 1.0f - uv.y });
		}
		else if (strcmp(buffer, "f") == 0)
		{
			uint32_t v[4], t[4], n[4];
			fgets(buffer, (int)std::size(buffer), file);
			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2],
				&v[3], &t[3], &n[3]) == 12)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);

				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
			}
			else  if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
			}
			else
			{
				ASSERT(false, "[ObjLoader] Unsupported OBJ format. File: %s.", fileName.u8string().c_str());
			}
		}
	}
	fclose(file);

	//Reconstruct Vertices
	std::vector<VertexPX> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0; i < positionIndices.size(); i++)
	{
		VertexPX vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.uv = uvs[uvIndices[i] - 1];
		vertices.emplace_back(vertex);
	}
	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);// fil 0,1,2,3,, .....

	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);
}

void ModelLoader::LoadObj(std::filesystem::path fileName, float scale, Mesh& mesh)
{
	std::vector<uint32_t> positionIndices, normalIndices, uvIndices;
	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector3> normals;
	std::vector<Math::Vector2> uvs;

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "[ModelLoader] Failed to open file %s.", fileName.u8string().c_str());

	while (true)
	{
		char buffer[128] = "";
		int res = fscanf_s(file, "%s", buffer, (unsigned int)std::size(buffer));
		if (res == EOF)
			break;

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back({ uv.x, 1.0f - uv.y });
		}
		else if (strcmp(buffer, "vn") == 0)
		{
			Math::Vector3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(buffer, "f") == 0)
		{
			uint32_t v[4], t[4], n[4];
			fgets(buffer, (int)std::size(buffer), file);
			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2],
				&v[3], &t[3], &n[3]) == 12)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);

				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[2]);
				normalIndices.push_back(n[3]);
			}
			else  if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);
			}
			else
			{
				ASSERT(false, "[ObjLoader] Unsupported OBJ format. File: %s.", fileName.u8string().c_str());
			}
		}
	}
	fclose(file);

	//Reconstruct Vertices
	std::vector<Vertex> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0; i < positionIndices.size(); i++)
	{
		Vertex vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.normal = normals[normalIndices[i] - 1];
		vertex.uv = uvs[uvIndices[i] - 1];
		vertices.emplace_back(vertex);
	}
	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);// fil 0,1,2,3,, .....

	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);
}

void ModelLoader::LoadModel(std::filesystem::path fileName, Model& model)
{

	printf("Loading model: %s....\n", fileName.u8string().c_str());
	
	FILE* file = nullptr;
	auto err = fopen_s(&file, fileName.u8string().c_str(), "r"); // r is read and w is write
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for loading. \n", fileName.u8string().c_str());
		return;
	}

	//assign the meshData in the size
	uint32_t meshCount;
	fscanf_s(file, "MeshCount:%u\n", &meshCount);
	//set the vercter size at mesh count but it is empty;
	model.meshData.reserve(meshCount);

	for (uint32_t i = 0; i < meshCount; ++i)
	{
		//set meshData[i] in the emplace_back 
		auto& meshData = model.meshData.emplace_back(std::make_unique<Model::MeshData>());
		fscanf_s(file, "MaterialIndex: %d\n", &meshData->materiaIndex);
		MeshIO::Read(file, meshData->mesh);
	}

	fclose(file);
}

void ModelLoader::LoadMaterial(std::filesystem::path fileName, Model& model)
{

	std::filesystem::path path = fileName;
	path.replace_extension("material");

	printf("Loading material: %s....\n", path.u8string().c_str());

	FILE* file = nullptr;
	auto err = fopen_s(&file, path.u8string().c_str(), "r");
	if (err != 0 || file == nullptr)
	{
		printf("Error:failed to open file %s for loading.\n", path.u8string().c_str());
		return;
	}

	uint32_t materialCount;
	fscanf_s(file, "MaterialCount:%d\n", &materialCount);
	model.materialData.resize(materialCount);

	for (auto& materialData : model.materialData)
	{
		MaterialIO::Read(file, materialData);
	}

	fclose(file);
}

void ModelLoader::LoadSkeleton(std::filesystem::path fileName, Model& model)
{
	std::filesystem::path path = fileName;
	path.replace_extension("skeleton");

	printf("Load skeleton: %s....\n", path.u8string().c_str());

	FILE* file = nullptr;
	auto err = fopen_s(&file, path.u8string().c_str(), "r");
	if (err != 0 || file == nullptr)
	{
		printf("Error:failed to open file %s for loading.\n", path.u8string().c_str());
		return;
	}

	model.skeleton = std::make_unique<Skeleton>();
	SkeletonIO::Read(file, *(model.skeleton));

	fclose(file);
}

void ModelLoader::LoadAnimation(std::filesystem::path fileName, Model& model)
{
	std::filesystem::path path = fileName;
	path.replace_extension("animset");

	printf("Load animation: %s....\n", path.u8string().c_str());

	FILE* file = nullptr;
	auto err = fopen_s(&file, path.u8string().c_str(), "r");
	if (err != 0 || file == nullptr)
	{
		printf("Error:failed to open file %s for loading.\n", path.u8string().c_str());
		return;
	}

	AnimationIO::Read(file, model.animSet);

	fclose(file);
}


