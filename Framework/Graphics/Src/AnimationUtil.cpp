#include "Precompiled.h"
#include "AnimationUtil.h"
#include "SimpleDraw.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Math;

namespace
{
    void DrawBone(Bone* bone, const std::vector<Math::Matrix4>& boneMatrices)
    {
        Math::Vector3 bonePosition = GetTranslation(boneMatrices[bone->index]);
        for (size_t i = 0; i < bone->children.size(); i++)
        {
            Math::Vector3 childPosition = GetTranslation(boneMatrices[bone->children[i]->index]);
            //auto direction = childPosition - bonePosition;
            //SimpleDraw::AddBone(bonePosition, direction, Colors::AliceBlue, scale * 0.01f, true);
            SimpleDraw::AddLine(bonePosition, childPosition, Colors::Blue);
            DrawBone(bone->children[i], boneMatrices);
        }
    }

    void GetRootTransformRecursive(Bone* bone, std::vector<Math::Matrix4>& toRootTransform)
    {
        if (bone->parent)
        {
            toRootTransform[bone->index] = bone->toParentTransform * toRootTransform[bone->parentIndex];
        }
        for (size_t i = 0; i < bone->children.size(); i++)
        {
            GetRootTransformRecursive(bone->children[i], toRootTransform);
        }
    }

    void GetAnimationTransformRecursive(Bone* bone, const AnimationClip& clip, float time, std::vector<Math::Matrix4>& toRootTransform)
    {
        if (bone->parent)
        {
            auto& boneAnimation = clip.boneAnimations[bone->index];
            if (boneAnimation)
            {
                auto position = boneAnimation->GetPosition(time);
                auto rotation = boneAnimation->GetRotation(time);
                auto scale = boneAnimation->GetScale(time);
                auto transform = Matrix4::Scaling(scale) * Matrix4::RotationQuaternion(rotation) * Matrix4::Translation(position);
                toRootTransform[bone->index] = transform * toRootTransform[bone->parentIndex];
            }
            else
            {
                toRootTransform[bone->index] = bone->toParentTransform * toRootTransform[bone->parentIndex];
            }
        }
        for (size_t i = 0; i < bone->children.size(); i++)
        {
            GetAnimationTransformRecursive(bone->children[i], clip, time, toRootTransform);
        }
    }
}

void Graphics::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices)
{
    // go each of boneMatrices 
    DrawBone(skeleton.root, boneMatrices);
}

void Graphics::GetRootTransform(const Skeleton& skeleton, std::vector<Math::Matrix4>& toRootTransfrom)
{
    toRootTransfrom.resize(skeleton.bones.size());
    GetRootTransformRecursive(skeleton.root, toRootTransfrom);
}

void Graphics::GetAnimationTransForm(const Skeleton& skeleton, const AnimationClip& clip, float time, std::vector<Math::Matrix4>& toRootTransfrom)
{
    toRootTransfrom.resize(skeleton.bones.size());
    GetAnimationTransformRecursive(skeleton.root, clip, time, toRootTransfrom);
}
