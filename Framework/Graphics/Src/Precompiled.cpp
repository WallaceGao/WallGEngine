#include "Precompiled.h"

//COM- Componet Object Model(MS Tech)
// i)It handles memory management for you
//      - instead of new/delete, you ask for objects using Create<***> function, and call Release when you are done
// ii) Everything is In terface based

// What is an Interface
//  a list of public pure virtual function declerations
// cab allow system level encapus

//ID3D11Device* d;         // Allows us to create d3d resources
//ID3D11DeviceContext* c;  // Allows us to sent draw commands
//
//class SwapChain
//{
//	float* backbuffers = new float[1920 * 1080 * 3];
//};

//float* backbuffer = new float[1920 * 1080 * 3];
//float* displaybuffer = new float[1920 * 1080 * 3];
//clear(backbuffer);

//for (int y = 0; y < length; ++y)
//{
//	for (int x = 0; x < length; ++x)
//	{
//		backbuffer[x] = r;
//		backbuffer[x] = g;
//		backbuffer[x] = b;
//	}
//}
//
//memcpy(displaybuffer, backbuffer, ...);
//
//monitor.Display(displaybuffer);