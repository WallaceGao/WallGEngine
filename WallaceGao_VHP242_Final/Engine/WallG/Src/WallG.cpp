#include "Precompiled.h"
#include "WallG.h"

WallG::App& WallG::MainApp()
{
	static App sApp;
	return sApp;
}