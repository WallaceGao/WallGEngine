#pragma once

#include "Common.h"
#include "Colors.h"

namespace WallG::Graphics
{

	// Vertex element flags
	constexpr uint32_t VE_Position = 0x1 << 0;
	constexpr uint32_t VE_Normal = 0x1 << 1;
	constexpr uint32_t VE_Tangent = 0x1 << 2;
	constexpr uint32_t VE_Color = 0x1 << 3;
	constexpr uint32_t VE_TexCoord = 0x1 << 4;
	constexpr uint32_t VE_BlendIndex = 0x1 << 5;
	constexpr uint32_t VE_BlendWeight = 0x1 << 6;

	#define VERTEX_FORMAT(fmt)\
        static constexpr uint32_t Format = fmt

	struct  VertexPC 
	{
		VERTEX_FORMAT(VE_Position | VE_Color);
		Math::Vector3 position;
		Color color;
	};

	struct  VertexPX 
	{
		VERTEX_FORMAT(VE_Position | VE_TexCoord);
		Math::Vector3 position;
		Math::Vector2 uv;
	};

	struct  VertexPCX
	{
		VERTEX_FORMAT(VE_Position | VE_Color | VE_TexCoord);
		Math::Vector3 position;
		Color color;
		Math::Vector2 uv;
	};

}
