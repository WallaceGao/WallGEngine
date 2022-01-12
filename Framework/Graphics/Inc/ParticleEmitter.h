#pragma once

#include "BlendState.h"
#include "DepthStencilState.h"
#include "Colors.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Texture.h"
#include "VertexShader.h"

namespace WallG::Graphics
{
	class Camera;
	
	struct Particle
	{
		Math::Vector3 position;
		Math::Vector3 velocity;
		Math::Vector3 acceleration;
		Color startColor, endColor;
		float startSize, endSize;
		float lifetime, maxLifetime;
		int textureIndex = 0;
	};
	
	class ParticleEmitter
	{
	public:
		void Initialize();
		void Terminate();
	
		void Start(float emitRate);
		void Stop();
	
		void Update(float deltaTime);
		void Render(const Camera& camera);
	
		void SetPosition(const Math::Vector3& pos = Math::Vector3::Zero);
		void SetDirection(const Math::Vector3& dir, float span);
	
		void SetStartColor(const Color& color) { mStartColor = color; }
		void SetEndColor(const Color& color) { mEndColor = color; }
		void SetStartSize(float size) { mStartSize = size; }
		void SetEndSize(float size) { mEndSize = size; }
		void SetMaxLifeTime(float maxLifeTime) { mMaxLifetime = maxLifeTime; }
	
	private:
		MeshBuffer mMeshBuffer;
		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		Texture mTexture;
		BlendState mBlendState;
		DepthStencilState mDepthStencilState;
	
		Math::Vector3 mPosition;
		Math::Vector3 mDirection;
		float mDirectionSpan = 0.0f;
		float mEmitRate = 0.0f;
		float mEmitCount = 0.0f;
		Color mStartColor = Colors::White, mEndColor = Colors::White;
		float mStartSize = 1.0f, mEndSize = 1.0f;
		float mMaxLifetime = 1.0f;
	
		std::vector<Particle> mParticles;
	};
}