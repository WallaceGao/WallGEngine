#include "PlanetComponent.h"

#include "UniverseService.h"

using namespace WallG;
using namespace WallG::Math;

MEMPOOL_DEFINE(PlanetComponent, 1000);

void PlanetComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mTransformComponent->SetScale(mPlanetScale);
	mRotation.y = mStartRota * Constants::DegToRad;

	UniverseService* universeService = GetOwner().GetWorld().GetService<UniverseService>();
	universeService->Register(this);
}

void PlanetComponent::Terminate()
{
	UniverseService* univerService = GetOwner().GetWorld().GetService<UniverseService>();
	univerService->Unregister(this);
}

void PlanetComponent::Update(float deltaTime)
{
	Math::Matrix4 parentMatrix = Math::Matrix4::Indentity;
	
	// check parent planet
	GameObject* parentObject = GetOwner().GetWorld().GetGameObject(mParentPlanet);
	if (parentObject != nullptr)
	{
		TransformComponent* parentTransform = parentObject->GetComponent<TransformComponent>();
		parentMatrix =
			Math::Matrix4::RotationX(parentTransform->GetRotation().x) *
			Math::Matrix4::RotationY(parentTransform->GetRotation().y) *
			Math::Matrix4::RotationZ(parentTransform->GetRotation().z) *
			Math::Matrix4::Translation(parentTransform->GetPosition());
	}
	mRotation.y -= deltaTime * mSpeed;
	mSelfRotation.y -= deltaTime * mSelfSpeed;

	Math::Matrix4 myMatrix =
		Matrix4::Scaling(mTransformComponent->GetScale()) *
		Matrix4::RotationY(mSelfRotation.y) *
		Matrix4::Translation({ mDistanceFromParent, 0.0f, 0.0f }) *
		Matrix4::RotationY(mRotation.y);

	Math::Matrix4 finalMatrix = myMatrix * parentMatrix;

	Math::Vector3 position = Math::GetTranslation(finalMatrix);
	Math::Quaternion rotation = Math::Quaternion::RotationMatrix(finalMatrix);
	mTransformComponent->SetPosition(position);
	mTransformComponent->SetRotation(rotation);
	mTransformComponent->SetScale(Vector3(mTransformComponent->GetScale()));

}

void PlanetComponent::DebugUI()
{
	if (ImGui::CollapsingHeader("Planet", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(" TransForm: %f, %f, %f ", mTransformComponent->GetPosition().x , mTransformComponent->GetPosition().y, mTransformComponent->GetPosition().z);
		ImGui::DragFloat(" Rota Speed", (float*)&mSpeed, 0.01f);
		ImGui::DragFloat("Self Rota Speed", (float*)&mSelfSpeed, 0.01f);
		if (mResourceType == MineralType::Iron)
		{
			ImGui::Text("Resource Type: Iron");
		}
		if (mResourceType == MineralType::Copper)
		{
			ImGui::Text("Resource Type: Copper");
		}
		if (mResourceType == MineralType::None)
		{
			ImGui::Text("Resource Type: None");
		}
	}
}



