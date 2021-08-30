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
		WallG::Math::Matrix4 world;
		WallG::Math::Matrix4 wvp;
		WallG::Math::Vector3 viewPostion;
		float padding;
	};

	struct Settings
	{
		float specularMapWeight = 1.0f;
		float bumpMapWeight = 0.2f;
		float normalMapWeight = 1.0f;
		int useShadow = 0;
		int useSkinning = 0;
		float padding[3];
	};
	struct StandardBoneTransformData
	{
		WallG::Math::Matrix4 boneTransform[256];
	};

	WallG::Graphics::Camera mCamera;

	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
	WallG::Graphics::TypedConstantBuffer<Settings> mSettingsBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::DirectionalLight> mLightBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;

	WallG::Graphics::DirectionalLight mDirctionalLight;
	WallG::Graphics::Material mMaterial;
	// character
	WallG::Graphics::Model mGreatSwordStrafe;
	WallG::Graphics::Animator mAnimator;
	WallG::Graphics::TypedConstantBuffer<StandardBoneTransformData> mBoneTransformBuffer;

	WallG::Graphics::VertexShader mVertexShader;
	WallG::Graphics::PixelShader mPixelShader;

	WallG::Graphics::Sampler mSampler;

	Settings mSettings;
	WallG::Math::Vector3 mPosition;
	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;
	bool mShowAnimation = false;
	bool mShowSkelton = false;
	bool mIsLoop = false;
};

