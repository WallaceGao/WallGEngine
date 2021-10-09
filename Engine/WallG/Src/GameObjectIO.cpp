#include "Precompiled.h"
#include "GameObjectIO.h"

#include "GameObject.h"

#include "AnimatorComponent.h"
#include "ColliderComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>

using namespace WallG;
using namespace rapidjson;

namespace
{
	WallG::GameObjectIO::ReadOverrideFunc readOverride;
}

void WallG::GameObjectIO::SetReadOverride(ReadOverrideFunc func)
{
	readOverride = func;
}

void WallG::GameObjectIO::Wirte(FILE* file, const GameObject& gameObject)
{
	// TODO
}

void WallG::GameObjectIO::Read(FILE* file, GameObject& gameObject)
{
	char readBuffer[60000];
	FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	
	Document document;
	document.ParseStream(readStream);

	auto components = document["Components"].GetObj();
	for (auto& component: components)
	{ 
		const char* componentName = component.name.GetString();
		if (readOverride && readOverride( componentName, component, gameObject))
		{
			continue;
		}
		else if (strcmp(componentName, "TransformComponent")==0)
		{
			auto transform = gameObject.AddComponent<TransformComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& position = component.value["Position"].GetArray();
				float x = position[0].GetFloat();
				float y = position[1].GetFloat();
				float z = position[2].GetFloat();
				transform->SetPosition({x,y,z});
			};
			if (component.value.HasMember("Scale"))
			{
				const auto& scale = component.value["Scale"].GetArray();
				float x = scale[0].GetFloat();
				float y = scale[1].GetFloat();
				float z = scale[2].GetFloat();
				transform->SetScale({ x,y,z });
			};
		}
		else if(strcmp(componentName, "ColliderComponent") == 0)
		{
			auto collider = gameObject.AddComponent<ColliderComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				float x = center[0].GetFloat();
				float y = center[1].GetFloat();
				float z = center[2].GetFloat();
				collider->SetCenter({x,y,z});
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				float x = extend[0].GetFloat();
				float y = extend[1].GetFloat();
				float z = extend[2].GetFloat();
				collider->SetExtend({ x,y,z });
			}
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			auto Model = gameObject.AddComponent<ModelComponent>();
			if (component.value.HasMember("Path"))
			{
				const auto& path = component.value["Path"].GetString();
				Model->SetFilePath(path);
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& eulerAngles = component.value["Rotation"].GetArray();
				float x = eulerAngles[0].GetFloat();
				float y = eulerAngles[1].GetFloat();
				float z = eulerAngles[2].GetFloat();
				Model->SetRotation(Math::Quaternion::RotationEuler({ x, y, z }));
			}
			if (component.value.HasMember("Ground"))
			{
				const auto& isGround = component.value["Ground"].GetBool();
				Model->SetGround(isGround);
			}
		}
		else if (strcmp(componentName, "AnimatorComponent") == 0)
		{
			auto Animator = gameObject.AddComponent<AnimatorComponent>();
			//if (component.value.HasMember("ShowAnimation"))
			//{
			//	const auto& showAnimation = component.value["ShowAnimation"].GetBool();
			//	collider->SetShowAnimation(showAnimation);
			//}
			//if (component.value.HasMember("ShowSkeleton"))
			//{
			//	const auto& showSkeleton = component.value["ShowSkeleton"].GetBool();
			//	collider->SetShowSkeleton(showSkeleton);
			//}
			if (component.value.HasMember("IsLoop"))
			{
				const auto& IsLoop = component.value["IsLoop"].GetBool();
				Animator->SetLoop(IsLoop);
			}
		}
	}
}
