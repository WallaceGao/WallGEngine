#include "Precompiled.h"
#include "SkeletonIO.h"

using namespace WallG::Graphics;

namespace
{
	void readMatrix(FILE* file, WallG::Math::Matrix4& transform)
	{
		fscanf_s(file, "%f %f %f %f \n", &transform._11, &transform._12, &transform._13, &transform._14);
		fscanf_s(file, "%f %f %f %f \n", &transform._21, &transform._22, &transform._23, &transform._24);
		fscanf_s(file, "%f %f %f %f \n", &transform._31, &transform._32, &transform._33, &transform._34);
		fscanf_s(file, "%f %f %f %f \n", &transform._41, &transform._42, &transform._43, &transform._44);
	}
	void writeMatrix(FILE* file, WallG::Math::Matrix4 transform)
	{
		fprintf_s(file, "%f %f %f %f \n", transform._11, transform._12, transform._13, transform._14);
		fprintf_s(file, "%f %f %f %f \n", transform._21, transform._22, transform._23, transform._24);
		fprintf_s(file, "%f %f %f %f \n", transform._31, transform._32, transform._33, transform._34);
		fprintf_s(file, "%f %f %f %f \n", transform._41, transform._42, transform._43, transform._44);
	}

}


void SkeletonIO::Write(FILE* file, const Skeleton& skeleton)
{
	const uint32_t BoneCount = static_cast<uint32_t>(skeleton.bones.size());
	fprintf_s(file, "BonesSize: %d\n", BoneCount);
	for (uint32_t i = 0; i < skeleton.bones.size(); ++i)
	{
		auto& bone = skeleton.bones[i];
		fprintf_s(file, "ParentIndex: %d\n", bone->parentIndex);
		fprintf_s(file, "Name: %s\n", bone->name.c_str());
		fprintf_s(file, "Index: %d\n", bone->index);
		fprintf_s(file, "ChildCount: %u\n", (uint32_t)bone->children.size());
		for (uint32_t j = 0; j < bone->children.size(); ++j)
		{
			fprintf_s(file, "Child Indicies: %d\n",bone->childIndices[j] );
		}
		writeMatrix(file, bone->toParentTransform);
		writeMatrix(file, bone->offsetTransform);
	}
}

void SkeletonIO::Read(FILE* file, Skeleton& skeleton)
{
	uint32_t BoneSize = 0;

	fscanf_s(file, "BonesSize: %d\n", &BoneSize);
	skeleton.bones.reserve(BoneSize);

	for (uint32_t i = 0; i < BoneSize; ++i)
	{
		auto bone = std::make_unique<Bone>();
		char name[128];
		uint32_t index =0;
		fscanf_s(file, "ParentIndex: %d\n", &bone->parentIndex);
		fscanf_s(file, "Name: %s\n", name, static_cast<uint32_t>(sizeof(name)));
		bone->name = name;
		fscanf_s(file, "Index: %d\n", &index);
		bone->index = index;
		uint32_t childCount = 0;
		fscanf_s(file, "ChildCount: %u\n", &childCount);
		
		for (uint32_t j	 = 0; j < childCount; ++j)
		{
			int boneIndex = 0;
			fscanf_s(file, "Child Indicies: %d\n", &boneIndex);
			bone->childIndices.push_back(boneIndex);
		}
		readMatrix(file, bone->toParentTransform);
		readMatrix(file, bone->offsetTransform);
		skeleton.bones.push_back(std::move(bone));
	}
}
