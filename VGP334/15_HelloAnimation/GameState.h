#pragma once
#include <WallG/Inc/WallG.h>

class GameState: public WallG::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
private:
	struct TransformData
	{
		WallG::Math::Matrix4 wvp;
	};

	WallG::Graphics::Camera mCamera;

	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;

	WallG::Graphics::MeshPX mMesh;
	WallG::Graphics::MeshBuffer mMeshBuffer;

	WallG::Graphics::VertexShader mVertexShader;
	WallG::Graphics::PixelShader mPixelShader;

	WallG::Graphics::Sampler mSampler;
	WallG::Graphics::Texture mTexture;

	WallG::Math::Vector3 mRotation;

	WallG::Graphics::Animation mAnimation;
	float mAnimationTimer = 0.0f;
	float mFPS = 0.0f;
};


// For homework:
// - Add a new project called HelloAnimation.
// - Use the AnimationBuilder to create a new animation with all key frame types.
// - Setup a scene where you have objects moving which get transforms from the animation.
// - You will need to keep a running time to retrieve animation keys.
//
// e.g.
//
//    float mAnimationTimer = 0.0f;
//
//    void Update()
//    {
//        mAnimationTimer += deltaTime;
//        ...
//    }
//
//    void Render()
//    {
//        ...
//        const auto pos = mAnimation.GetPosition(mAnimationTimer);
//        const auto rot = mAnimation.GetRotation(mAnimationTimer);
//        const auto scale = mAnimation.GetScale(mAnimationTimer);
//        const auto matWorld = Matrix4::Scaling(scale) * Matrix4::RotationQuaternion(rot) * Matrix4::Translation(pos);
//        ...
//    }


