#include "Precompiled.h"
#include "ParticleComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "RenderService.h"

using namespace WallG;
MEMPOOL_DEFINE(ParticleComponent, 1000);

void ParticleComponent::Initialize()
{

}

void ParticleComponent::Terminate()
{
	for (size_t i = 0; i < mParticleEmitters.size(); ++i)
	{
		mParticleEmitters[i].Terminate();
	}

}

void ParticleComponent::Update(float deltaTime)
{
	for (size_t i = 0; i < mParticleEmitters.size(); ++i)
	{
		mParticleEmitters[i].Update(deltaTime);
	}
}

void ParticleComponent::DebugUI()
{
	
}

void ParticleComponent::AddNewParticle(ParticleSetting particleSetting)
{
	Graphics::ParticleEmitter newParticleEmitter;
	newParticleEmitter.Initialize();
	newParticleEmitter.SetPosition(particleSetting.mPosition);
	newParticleEmitter.SetStartSize(particleSetting.mStartSize);
	newParticleEmitter.SetEndSize(particleSetting.mEndSize);
	newParticleEmitter.SetMaxLifeTime(particleSetting.mMaxLifeTime);
	newParticleEmitter.SetStartColor(particleSetting.mStartColor);
	newParticleEmitter.SetEndColor(particleSetting.mEndColor);
	newParticleEmitter.Start(90.0f);
	mParticleEmitters.push_back(newParticleEmitter);
}
