#include <WallG/Inc/WallG.h>

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	

	WallG::Core::Window window;
	
	// char - 1 byte - 8 bits - can store 0000 0000 or 1111 1111 or 255,
	// wchar - 2 byte - 16 bits - unicode characters
	window.Initialize(hInstance, L"Hellow Window", 1280, 720);

	bool running = true;
	while (running && window.IsActive())
	{

		window.ProcessMessage();
		if (!window.IsActive())
		{
			continue;
		}
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			running = false;
		}
	}

	//clearn up
	window.Terminate();
	return 0;
}