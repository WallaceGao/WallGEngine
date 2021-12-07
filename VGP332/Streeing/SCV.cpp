#include "SCV.h"
#include "TypeIds.h"


using namespace WallG;
using namespace WallG::AI;
using namespace WallG::Graphics;
using namespace WallG::Math;

SCV::SCV(AI::AIWorld& world)
    : AI::Agent(world, TypeIds::SCV)
{
}

void SCV::Initialize()
{
    maxSpeed = 200.0f;

    mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
    mSeekBehavior =  mSteeringModule->AddBehavior<AI::SeekBehavior>();
    mSeekBehavior->SetActive(isSeek);
    mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();
    mWanderBehavior->SetActive(isWander);
    mObstacleAvoidence = mSteeringModule->AddBehavior<AI::ObstacleAvoidance>();
    mObstacleAvoidence->SetActive(isObstacle);

    for (size_t i = 0; i < mTextureIds.size(); ++i)
    {
        char name[256];
        sprintf_s(name, "carrier_%02zu.png", i + 1);
        mTextureIds[i] = TextureManager::Get()->LoadTexture(name);
    }
    radius = SpriteRenderer::Get()->GetSpriteWidth(mTextureIds[0])*0.5f;
}

void SCV::Update(float deltaTime)
{
    auto force = mSteeringModule->Calculate();
    auto acceleration = force / mass;
    velocity += acceleration * deltaTime;
    // TODO - Add truncate to max speed here
    position += velocity * deltaTime;

    if (Math::MagnitudeSquared(velocity) > 1.0f)
        heading = Math::Normalize(velocity);

    // TODO - Add wrapping logic for all sides here
    auto screenWidth = GraphicsSystem::Get()->GetBackBufferWidth();
    auto screenHight = GraphicsSystem::Get()->GetBackBufferHeight();
    if (position.x >= screenWidth)
    {
        position.x -= screenWidth;
    }
    if (position.y >= screenHight)
    {
        position.y -= screenHight;
    }
    if (position.x <= 0)
    {
        position.x += screenWidth;
    }
    if (position.y <= 0)
    {
        position.y += screenHight;
    }
}

void SCV::Render()
{
    float angle = atan2(-heading.x, heading.y) + Math::Constants::Pi;
    int frame = (int)(angle / Math::Constants::TwoPi * mTextureIds.size()) % mTextureIds.size();
    SpriteRenderer::Get()->DrawSprite(mTextureIds[frame], position);
}

void SCV::ShowDebugUI()
{
    ImGui::Begin("Behavior");
    if (ImGui::Checkbox("Seek", &isSeek))
    {
        mSeekBehavior->SetActive(isSeek);
    }
    if (ImGui::Checkbox("Wander", &isWander))
    {
        mWanderBehavior->SetActive(isWander);
    }
    if (ImGui::Checkbox("Obstacle", &isObstacle))
    {
        mObstacleAvoidence->SetActive(isObstacle);
    }
    ImGui::End();
}
