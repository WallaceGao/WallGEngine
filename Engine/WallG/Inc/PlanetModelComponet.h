#pragma once
#include "ModelComponent.h"


namespace WallG
{
	class PlanetComponent final : public ModelComponent
	{
	public:
		SET_TYPE_ID(ComponentId::PlanetModel);
		MEMPOOL_DECLARE;

		PlanetComponent() = default;

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void SetTextureFilePath(const char* fileName) { mTextureFilePath = fileName; }
		
		void SetSelfRotation(const Math::Vector3& rotation) { mSelfRotation = rotation; }
		const Math::Vector3& GetSelfRotation() const { return mSelfRotation; }
		void SetSpeed(float speed) { mSpeed = speed; }
		const float GetSpeed() const { return mSpeed; }
		void SetSelfSpeed(float selfSpeed) { mSelfSpeed = selfSpeed; }
		const float GetSelfSpeed() const { return mSelfSpeed;}

		void Rota(float speed) { mSelfRotation -= speed; }

	protected:
		WallG::Math::Vector3 mSelfRotation;
		std::string mTextureFilePath;
		float mSpeed = 0.0f;
		float mSelfSpeed = 0.0f;
	};
};

