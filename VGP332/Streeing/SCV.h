#pragma once
#include "WallG/Inc/WallG.h"

class SCV : public AI::Agent
{
public:
    SCV(AI::AIWorld& world);

    void Initialize();

    void Update(float deltaTime);
    void Render();

    void ShowDebugUI();

private:
    std::unique_ptr<AI::SteeringModule> mSteeringModule;
    std::array<Graphics::TextureId, 32> mTextureIds;

    AI::SeekBehavior* mSeekBehavior;
    AI::WanderBehavior* mWanderBehavior;
    AI::ObstacleAvoidance* mObstacleAvoidence;

    bool isSeek = false;
    bool isWander = false;
    bool isObstacle = false;
};

