#include "GameState.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });
    //mAnimator.Initialize( &mGreatSwordStrafe);
    mPhysicsWorld.Initialize();
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
    auto inputSystem = InputSystem::Get();

    const float moveSpeed = 10.0f;
    const float turnSpeed = 10.0f * Constants::DegToRad;
    if (inputSystem->IsKeyDown(KeyCode::W))
        mCamera.Walk(moveSpeed * deltaTime);
    if (inputSystem->IsKeyDown(KeyCode::S))
        mCamera.Walk(-moveSpeed * deltaTime);
    if (inputSystem->IsKeyDown(KeyCode::D))
        mCamera.Strafe(moveSpeed * deltaTime);
    if (inputSystem->IsKeyDown(KeyCode::A))
        mCamera.Strafe(-moveSpeed * deltaTime);
    if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
    }

    mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
    mPhysicsWorld.DebugDraw();

    //SimpleDraw::addPlane();

    // Attach buffer to graphics pipeline
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();

    SimpleDraw::AddLine(Vector3::Zero, Vector3::XAxis, Colors::Red);
    SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis, Colors::Green);
    SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis, Colors::Blue);
    SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button("Boom!"))
    {
        mPhysicsWorld.Clear();
        for (int i = 0; i < 100; ++i)
        {
            Vector3 beginPoint = Vector3{0.0f,0.0f,0.0f};
            Vector3 endPoint;
            endPoint.x = Math::RandomFloat(-1.0f, 1.0f);
            endPoint.y = Math::RandomFloat( 0.0f, 1.0f);
            endPoint.z = Math::RandomFloat(-1.0f, 1.0f);
            endPoint *= 0.1f;

            auto particle = mPhysicsWorld.AddParticle();
            particle->SetPosition(beginPoint);
            particle->SetVelocity(endPoint);
            particle->radius = Math::RandomFloat(0.025f, 0.05f);
            particle->invMass = 1.0f;
        }
    }

    ImGui::End();
}
