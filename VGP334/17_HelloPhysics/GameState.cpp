#include "GameState.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -8.0f });
    mCamera.SetDirection({ 0.0f,1.0f,0.0f });

    WallG::Physics::PhysicsWorld::Settings setting
    {
        {0.0,-9.8f,0.0f},
        1.0f / 60.0f,
        10
    };

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
            Vector3 vel1 = Random::OnUnitSphere()*0.1f;
            vel1.y = Abs(vel1.y);
            Vector3 vel2 = Random::OnUnitSphere() * 0.1f;
            vel2.y = Abs(vel2.y);

            auto particle1 = mPhysicsWorld.AddParticle();
            particle1->SetPosition({+0.1f,1.0f,0.0f});
            particle1->SetVelocity(vel1);
            particle1->radius = 0.02f;
            particle1->invMass = 1.0f;
        }
    }

    if (ImGui::Button("Sticks!"))
    {
        mPhysicsWorld.Clear();
        for (int i = 0; i < 100; ++i)
        {
            Vector3 vel1 = Random::OnUnitSphere() * 0.1f;
            vel1.y = Abs(vel1.y);
            Vector3 vel2 = Random::OnUnitSphere() * 0.1f;
            vel2.y = Abs(vel2.y);

            auto particle1 = mPhysicsWorld.AddParticle();
            particle1->SetPosition({ +0.1f,1.0f,0.0f });
            particle1->SetVelocity(vel1);
            particle1->radius = 0.02f;
            particle1->invMass = 1.0f;

            auto particle2 = mPhysicsWorld.AddParticle();
            particle2->SetPosition({ -0.1f,1.0f,0.0f });
            particle2->SetVelocity(vel2);
            particle2->radius = 0.02f;
            particle2->invMass = 1.0f;

            mPhysicsWorld.AddConstraint<Physics::Spring>(particle1, particle2);
        }
    }

    if (ImGui::Button("Tetrahedron!"))
    {
        mPhysicsWorld.Clear();
        for (int i = 0; i < 50; ++i)
        {
            auto p1 = mPhysicsWorld.AddParticle();
            p1->SetPosition({ 0.0f,1.0f,0.0f });
            p1->radius = 0.02f;
            p1->invMass = 1.0f;

            auto p2 = mPhysicsWorld.AddParticle();
            p2->SetPosition({ 0.0f,0.8f,0.1f });
            p2->radius = 0.02f;
            p2->invMass = 1.0f;

            auto p3 = mPhysicsWorld.AddParticle();
            p3->SetPosition({ 0.1f,0.8f,-0.1f });
            p3->radius = 0.02f;
            p3->invMass = 1.0f;

            auto p4 = mPhysicsWorld.AddParticle();
            p4->SetPosition({ -0.1f,0.8f,-0.1f });
            p4->radius = 0.02f;
            p4->invMass = 1.0f;

            Vector3 vel1 = Random::OnUnitSphere() * 0.25f;
            vel1.y = Abs(vel1.y);
            p1->SetVelocity(vel1);

            mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p2, 0.2f);
            mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p3, 0.2f);
            mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p4, 0.2f);
            mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p3, 0.2f);
            mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p4, 0.2f);
            mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p4, 0.2f);
        }
    }

    if (ImGui::Button("Ball and Chain!"))
    {
        mPhysicsWorld.Clear();
    
        Vector3 position{ 0.0f,6.0f,0.0f };
        Physics::Particle* last = nullptr;
    
        for (int i = 0; i < 10; ++i)
        {
            auto p1 = mPhysicsWorld.AddParticle();
            p1->SetPosition(position);
            p1->SetVelocity(Vector3::Zero);
            p1->radius = (i == 9) ? 0.1f : 0.02f;
            p1->invMass = (i == 9) ? 5.0f : 1.0f;
    
            if (i == 0)
            {
                mPhysicsWorld.AddConstraint<Physics::Fixed>(p1);
            }
            else
            {
                mPhysicsWorld.AddConstraint<Physics::Spring>(p1, last);
            }
            position.x += 0.5f;
            last = p1;
        }
    }

    if (ImGui::Button("cube"))
    {

    }

    if (ImGui::Button("cloth"))
    {
        mPhysicsWorld.Clear();
        
        const int num = 10 ;
        std::vector<Vector3> columns;
        std::vector<Physics::Particle*> lasts;
        std::vector<Physics::Particle*> fronts;

        float distence = 0.5;
        float startPosition = 0.0f;
        for (size_t i = 0; i < num; ++i)
        {
            if (i == 0)
                columns.push_back({ startPosition , 6.0f , 0.0f });
            else
                columns.push_back({startPosition += distence , 6.0f , 0.0f });
            Physics::Particle* last = nullptr;
            Physics::Particle* front = nullptr;
            lasts.push_back(last);
            fronts.push_back(front);
        }

        for (size_t i = 0; i < 10; ++i)
        {
            for (size_t j = 0; j < num; ++j)
            {
                auto p1 = mPhysicsWorld.AddParticle();
                p1->SetPosition(columns[j]);
                p1->SetVelocity(Vector3::Zero);
                p1->radius = 0.02f;
                p1->invMass = 1.0f;
                fronts[j] = p1;
                if (i == 0)
                {
                    mPhysicsWorld.AddConstraint<Physics::Fixed>(fronts[j]);
                }
            }

            for (size_t j = 0; j < num - 1; ++j)
            {
                mPhysicsWorld.AddConstraint<Physics::Spring>(fronts[j], fronts[j + 1]);
                if (i == 0)
                {
                    mPhysicsWorld.AddConstraint<Physics::Spring>(fronts[j], fronts[j + 1]);
                }
                else
                {
                    mPhysicsWorld.AddConstraint<Physics::Spring>(fronts[j], fronts[j + 1]);
                    mPhysicsWorld.AddConstraint<Physics::Spring>(fronts[j], lasts[j]);
                    mPhysicsWorld.AddConstraint<Physics::Spring>(fronts[j + 1], lasts[j + 1]);
                }
            }

            for (size_t j = 0; j < num; ++j)
            {
                columns[j].z += distence;
                lasts[j] = fronts[j];
            }
        }
    }

    mPhysicsWorld.DebugUI();
    ImGui::End();


}
