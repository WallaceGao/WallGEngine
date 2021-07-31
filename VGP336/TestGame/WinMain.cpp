#include "GameState.h"
#include <WallG/Inc/WallG.h>

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	WallG::MainApp().AddState<GameState>("GameState");
	WallG::MainApp().Run({L"Test Game"});
	return 0;
}

// For homework:
// - Update the cube geometry so we have unique uvs per face (24v, 36i)
// - Find cool texture
// - Show three cubes, each drawn using a different sampler to compare the filtering mode
// - Add a MeshBuilder helper class in Graphics lib to keep the reuseable mesh building code
//    class MeshBuilder
//    {
//    public:
//        // Add these
//        static MeshPX CreateCubePX();
//        static MeshPX CreatePlanePX(uint32_t rows, uint32_t columns, float spacing); // facing positive Y
//                    for (z in rows)
//                        for (x in columns)
//                            add vertices
//    };