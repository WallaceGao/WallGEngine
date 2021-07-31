#pragma once
#include "Service.h"

namespace WallG
{
	class TransformComponent;
	class PlanetComponent;

	class UniverseService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Universe)

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		//void Register(const ModelComponent* modelComponent);
		//void Unregister(const ModelComponent* modelComponent);

	private:
		struct Entry
		{
			PlanetComponent* planetModelComponent = nullptr;
			TransformComponent* transformComponent = nullptr;
		};

		struct TransformData
		{
			WallG::Math::Matrix4 wvp;
		};


		float mFPS = 0.0f;

		std::vector<float> mPlanetScale;

		// ship
		//WallG::Graphics::MeshPX mShipMesh;
		//WallG::Graphics::Texture mShipTexture;
		//WallG::Graphics::MeshBuffer mShipMeshBuffer;
		//
		//std::vector<WallG::Math::Vector3> mPlantPosition;
		//std::vector<WallG::Math::Vector3> mShipLocation;
		//WallG::Graphics::Sampler mSampler;
		//int targetPlanet = 3;
	};
}