#include "Precompiled.h"
#include "ParticleEmitter.h"

#include "Camera.h"

using namespace WallG;
using namespace WallG::Graphics;

void ParticleEmitter::Initialize()
{
	mMeshBuffer.Initialize(nullptr, sizeof(VertexPCX), 1000 * 6, true);

	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mVertexShader.Initialize("../../Assets/Shaders/Particles.fx", VertexPCX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Particles.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../../Assets/Images/Particle01.jpg");

	mBlendState.Initialize(BlendState::Mode::Additive);
	mDepthStencilState.Initialize(true, false);
}

void ParticleEmitter::Terminate()
{
	mDepthStencilState.Terminate();
	mBlendState.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();
	mMeshBuffer.Terminate();
}

void ParticleEmitter::Start(float emitRate)
{
	mEmitRate = emitRate;
}

void ParticleEmitter::Stop()
{
	mEmitRate = 0.0f;
}

void ParticleEmitter::Update(float deltaTime)
{
	// Spawn new particles
	mEmitCount += mEmitRate * deltaTime;
	int count = (int)mEmitCount;
	for (int i = 0; i < count; ++i)
	{
		Particle p;
		p.position = mPosition;
		p.position.x += Math::Random::RandomFloat(-0.2f, 0.2f);
		p.position.y += Math::Random::RandomFloat(-0.2f, 0.2f);
		p.position.z += Math::Random::RandomFloat(-0.2f, 0.2f);
		p.velocity = Math::Vector3::Zero;
		p.acceleration = Math::Vector3::Zero;
		p.startColor = mStartColor;
		p.endColor = mEndColor;
		p.startSize = mStartSize;
		p.endSize = mEndSize;
		p.lifetime = p.maxLifetime = Math::Random::RandomFloat(0.0f, mMaxLifetime);
		p.textureIndex = Math::Random::UniformInt(0, 4);
		mParticles.push_back(p);
	}
	mEmitCount -= count;

	// Update and prune existing particles
	for (auto& p : mParticles)
		p.lifetime -= deltaTime;

	auto isDead = [](auto& p) { return p.lifetime <= 0.0f; };
	mParticles.erase(std::remove_if(mParticles.begin(), mParticles.end(), isDead), mParticles.end());
}

void ParticleEmitter::Render(const Camera& camera)
{
	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetProjectionMatrix();
	auto transform = Math::Transpose(matView * matProj);
	mConstantBuffer.Update(&transform);
	mConstantBuffer.BindVS(0);

	mVertexShader.Bind();
	mPixelShader.Bind();

	std::vector<VertexPCX> vertices;

	Math::Vector2 uv[] = { {0.0f, 0.0f}, {0.5f, 0.0f}, {0.0f, 0.5f}, {0.5f, 0.5f} };

	Math::Vector3 cameraDir = camera.GetDirection();
	std::sort(mParticles.begin(), mParticles.end(), [&cameraDir](auto& a, auto& b)
	{
		auto s = Dot(a.position, cameraDir);
		auto t = Dot(b.position, cameraDir);
		return s > t;
	});

	Math::Vector3 cameraPos = camera.GetPosition();
	Math::Vector3 up = { matView._21, matView._22, matView._23 };
	for (int i = 0; i < 1000 && i < mParticles.size(); ++i)
	{
		auto& p = mParticles[i];
		auto dirToCam = Normalize(cameraPos - p.position);
		auto side = Cross(up, dirToCam);
		VertexPCX tl, tr, bl, br;
		float t = 1.0f - p.lifetime / p.maxLifetime;
		float s = Math::Lerp(p.startSize, p.endSize, t);
		Color c = Math::Lerp(p.startColor, p.endColor, t);
		tl.position = p.position + (side * s) + (up * s);
		tr.position = p.position - (side * s) + (up * s);
		bl.position = p.position + (side * s) - (up * s);
		br.position = p.position - (side * s) - (up * s);
		tl.color = c;
		tr.color = c;
		bl.color = c;
		br.color = c;
		tl.uv = uv[p.textureIndex];
		tr.uv = tl.uv + Math::Vector2{ 0.5f, 0.0f };
		bl.uv = tl.uv + Math::Vector2{ 0.0f, 0.5f };
		br.uv = tl.uv + Math::Vector2{ 0.5f, 0.5f };
		vertices.push_back(tl);
		vertices.push_back(tr);
		vertices.push_back(bl);
		vertices.push_back(tr);
		vertices.push_back(br);
		vertices.push_back(bl);
	}

	mMeshBuffer.Update(vertices.data(), (uint32_t)vertices.size());
	mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);

	mSampler.BindPS(0);
	mTexture.BindPS(0);

	mBlendState.Set();
	mDepthStencilState.Set();
	mMeshBuffer.Render();
	mDepthStencilState.ClearState();
	BlendState::ClearState();
}

void ParticleEmitter::SetPosition(const Math::Vector3& pos)
{
	mPosition = pos;
}

void ParticleEmitter::SetDirection(const Math::Vector3& dir, float span)
{
	mDirection = dir;
	mDirectionSpan = span;
}


// Alpha Blending Formula
// final color = (source color * source alpha) + (destination color * destination alpha)