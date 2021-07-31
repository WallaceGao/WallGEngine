#include "Precompiled.h"
#include "PlanetModelComponet.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

using namespace WallG;
using namespace WallG::Math;

MEMPOOL_DEFINE(PlanetComponent, 1000);

void WallG::PlanetComponent::Initialize()
{
	ModelComponent::Initialize();
	
	mModelId = ModelManager::Get()->NewEmptyModel(mName);
	Graphics::Model* model = const_cast<Graphics::Model*>(ModelManager::Get()->GetModel(mModelId));
	if (model->meshData.empty())
	{
		// If model is empty, manually make one
		auto& meshData = model->meshData.emplace_back(std::make_unique<Model::MeshData>());
		// 1.0f radius from json file
		meshData->meshBuffer.Initialize(MeshBuilder::CreatSphere(16,16,1.0f));
		// Add diffuse map here
		meshData->materiaIndex = 0;

		auto& materialData = model->materialData.emplace_back();
		materialData.diffuseMapName = mTextureFilePath;
		materialData.diffuseMap = std::make_unique<Graphics::Texture>();
		materialData.diffuseMap->Initialize(mTextureFilePath);
	}
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void WallG::PlanetComponent::Terminate()
{
	ModelComponent::Terminate();
	mSpeed = 0.0f;
	mSelfSpeed = 0.0f;
}

void WallG::PlanetComponent::Update(float deltaTime)
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
	
	mRotation -= deltaTime * mSpeed;
	mSelfRotation -= deltaTime * mSelfSpeed;

	Math::Matrix4 myMatrix =
		Matrix4::Scaling(mPlanetScale) *
		Matrix4::RotationY(mSelfRotation.y) *
		Matrix4::Translation({ mDistanceFromParent, 0.0f, 0.0f }) *
		Matrix4::RotationY(mRotation.y);
	
	Math::Matrix4 finalMatrix = myMatrix * parentMatrix;
	
	Math::Vector3 position = Math::GetTranslation(finalMatrix);
	Math::Quaternion rotation = Math::Quaternion::RotationMatrix(finalMatrix);
	mTransformComponent->SetPosition(position);
	mTransformComponent->SetRotation(rotation);
	mTransformComponent->SetScale(Vector3(mPlanetScale));
}

void WallG::PlanetComponent::DebugUI()
{
	ImGui::CollapsingHeader( mName.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
	ImGui::DragFloat(" Rota Speed", (float*)&mSpeed, 0.01f);
	ImGui::DragFloat("Self Rota Speed", (float*)&mSelfSpeed, 0.01f);
}
