#include "GameState.h"
#include <WallG/Inc/WallG.h>

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	WallG::MainApp().AddState<GameState>("GameState");
	WallG::MainApp().Run({L"Hello Final"});
	return 0;
}