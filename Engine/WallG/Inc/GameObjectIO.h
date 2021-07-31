#pragma once

#include <rapidjson/allocators.h>
#include <rapidjson/document.h>

namespace rapidjson
{
	using JsonMember = GenericMember<UTF8<>, RAPIDJSON_DEFAULT_ALLOCATOR>;
}

namespace WallG
{
	class GameObject;

	namespace GameObjectIO
	{
		using ReadOverrideFunc = std::function<bool(const char*, const rapidjson::JsonMember&, GameObject&)>;

		void SetReadOverride(ReadOverrideFunc func);

		void Wirte(FILE* file, const GameObject& gameObject);
		void Read(FILE* file, GameObject& gameObject);
	}
}