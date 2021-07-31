#include "Precompiled.h"
#include "TimeUtil.h"

namespace
{
    std::chrono::high_resolution_clock::time_point startTime;
    bool initialized = false;
}

void WallG::Core::TimeUtil::Initialize()
{
    if (!initialized)
    {
        startTime = std::chrono::high_resolution_clock::now();
        initialized = true;
    }
}

float WallG::Core::TimeUtil::GetTime()
{
    Initialize();
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0f;
}

float WallG::Core::TimeUtil::GetDeltaTime()
{
    Initialize();
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
    lastTime = currentTime;
    return deltaTime;
}

