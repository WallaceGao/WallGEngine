#pragma once

#include "MeshTypes.h"
#include "Material.h"

namespace WallG::Graphics
{
	class MeshBuilder
	{
	public:
		// for all 
		static Mesh CreatSphere(uint32_t rings, uint32_t slices, float radius);
		static Mesh CreatPlane(uint32_t rows, uint32_t columns, float spacing);
		static Mesh CreatCube();
		//  color
		static MeshPC CreatCubePC();
		// texture
		static MeshPX CreatCubePX();
		static MeshPX CreatPlanePX(uint32_t rows, uint32_t columns, float spacing);
		static MeshPX CreatCylinderPX(uint32_t rings, uint32_t slices, float height, float radius);
		static MeshPX CreatSpherePX(uint32_t rings, uint32_t slices, float radius);
		static MeshPX CreatSkyboxPX(float size);
		// nomal
		static MeshPN CreatCubePN();
		static MeshPN CreatSpherePN(uint32_t rings, uint32_t slices, float radius);

	private:
	};
}
