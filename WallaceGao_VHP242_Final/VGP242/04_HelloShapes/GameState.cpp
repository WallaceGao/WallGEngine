#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    // loop by vertexBugger's size



    // NDC - Normalized Device Coordinate
    //
    //            +-----------------+
    //           /                 /|
    //     1.0  +-----------------+ |
    //          |        ^        | |
    //          |        |        | |
    //          | <------+------> | |
    //          |        |        | | 1.0
    //          |        v        |/
    //    -1.0  +-----------------+ 0.0
    //      -1.0               1.0
    
    // heart
    mHeartVertices.push_back(VertexPC{ Vector3{ -0.3f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f, -0.5f, 0.0f }, Colors::Red });

    mHeartVertices.push_back(VertexPC{ Vector3{ -0.3f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{ -0.25f,  0.35f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f,  0.2f, 0.0f }, Colors::Red });

    mHeartVertices.push_back(VertexPC{ Vector3{ -0.25f,  0.35f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{ -0.05f,  0.35f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f,  0.2f, 0.0f }, Colors::Red });

    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.3f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f, -0.5f, 0.0f }, Colors::Red });

    mHeartVertices.push_back(VertexPC{ Vector3{  0.3f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.25f,  0.35f, 0.0f }, Colors::Red });

    mHeartVertices.push_back(VertexPC{ Vector3{  0.25f,  0.35f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.0f,  0.2f, 0.0f }, Colors::Red });
    mHeartVertices.push_back(VertexPC{ Vector3{  0.05f,  0.35f, 0.0f }, Colors::Red });

    //triforce

    mTriforceVertices.push_back(VertexPC{ Vector3{  0.00f, 0.5f, 0.0f}, Colors::Yellow });
    mTriforceVertices.push_back(VertexPC{ Vector3{  0.25f, 0.0f, 0.0f}, Colors::Yellow });
    mTriforceVertices.push_back(VertexPC{ Vector3{ -0.25f, 0.0f, 0.0f}, Colors::Yellow });
  
    mTriforceVertices.push_back(VertexPC{ Vector3{  0.25f,  0.0f, 0.0f}, Colors::Yellow });
    mTriforceVertices.push_back(VertexPC{ Vector3{  0.50f, -0.5f, 0.0f}, Colors::Yellow });
    mTriforceVertices.push_back(VertexPC{ Vector3{  0.00f, -0.5f, 0.0f}, Colors::Yellow });

    mTriforceVertices.push_back(VertexPC{ Vector3{ -0.25f,  0.0f, 0.0f}, Colors::Yellow });
    mTriforceVertices.push_back(VertexPC{ Vector3{  0.00f, -0.5f, 0.0f}, Colors::Yellow });
    mTriforceVertices.push_back(VertexPC{ Vector3{ -0.50f, -0.5f, 0.0f}, Colors::Yellow });

    //Wu Kong Staff
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.005f,  0.65f, 0.0f}, Colors::Brown });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.02f,   0.65f, 0.0f},  Colors::Brown });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.005f, -0.65f, 0.0f}, Colors::Brown });

    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.02f,   0.65f, 0.0f}, Colors::Brown });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.02f,  -0.65f, 0.0f}, Colors::Brown });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.005f, -0.65f, 0.0f}, Colors::Brown });
     
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.0f,  0.7f, 0.0f}, Colors::BurlyWood });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ 0.025f,  0.7f, 0.0f}, Colors::BurlyWood });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.0f, -0.7f, 0.0f}, Colors::BurlyWood });
   
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ 0.025f,  0.7f, 0.0f}, Colors::BurlyWood });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ 0.025f, -0.7f, 0.0f}, Colors::BurlyWood });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{   0.0f, -0.7f, 0.0f}, Colors::BurlyWood });
  
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{  0.03f,  0.85f, 0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ -0.005f,  0.7f,  0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ -0.005f,   0.85f, 0.0f}, Colors::Gold });
    
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{  0.03f,  0.85f, 0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{  0.03f,  0.85f, 0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{  0.03f,  0.7f, 0.0f}, Colors::Gold });
    
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{  0.03f,  -0.85f, 0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ -0.005f, -0.85f, 0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ -0.005f, -0.7f,  0.0f}, Colors::Gold });
     
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{  0.03f,  -0.85f, 0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{ -0.005f, -0.7f, 0.0f}, Colors::Gold });
    mWuKongStaffVertices.push_back(VertexPC{ Vector3{  0.03f,  -0.7f, 0.0f}, Colors::Gold });

    // -------------------------------------------
    // Create vertex buffer

    mVertexBuffer[0].Initialize(mHeartVertices.data(), sizeof(VertexPC), (uint32_t)mHeartVertices.size());
    mVertexBuffer[1].Initialize(mTriforceVertices.data(), sizeof(VertexPC), (uint32_t)mTriforceVertices.size());
    mVertexBuffer[2].Initialize(mWuKongStaffVertices.data(), sizeof (VertexPC), (uint32_t)mWuKongStaffVertices.size());

    // -------------------------------------------
    // Compile and create vertex shader
    mVertexShader.Initialize(L"../../Assets/Shaders/DoSomething.fx", VertexPC::Format);

    // -------------------------------------------
    // Compile and create pixel shader
    mPixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");
    // -------------------------------------------
}

void GameState::Terminate()
{
    for (int i = 0; i < std::size(mVertexBuffer); ++i)
        mVertexBuffer[i].Terminate();

    mVertexShader.Terminate();
    mPixelShader.Terminate();
}

void GameState::Update(float deltaTime)
{
    auto inputSystem = InputSystem::Get();
    if (inputSystem->IsKeyPressed(WallG::Input::KeyCode::ONE))
    {
        input = 0;
    }

    if (inputSystem->IsKeyPressed(WallG::Input::KeyCode::TWO))
    {
        input = 1;
    }

    if (inputSystem->IsKeyPressed(WallG::Input::KeyCode::THREE))
    {
        input = 2;
    }
}

void GameState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mVertexBuffer[input].Render();
}

// Device interface - used for VRAM allocation/resource creation
// Context interface - used to issue draw commands