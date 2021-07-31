#pragma once

#include "MeshTypes.h"

namespace WallG::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPC CreatCubePC();
		static MeshPX CreatCubePX();
		static MeshPX CreatPlanePX(uint32_t rows, uint32_t columns, float spacing);
		static MeshPX CreatCylinderPX(uint32_t rings, uint32_t slices, float height, float radius);
		static MeshPX CreatSphere(uint32_t rings, uint32_t slices, float radius);
		static MeshPX CreatSkybox(float size);
	private:
	};
}
