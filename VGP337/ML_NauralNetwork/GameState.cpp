#include "GameState.h"
#include <ImGui/Inc/ImPlot.h>

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;
using namespace WallG::ML;

void GameState::Initialize()
{
    auto graphicsSystem = GraphicsSystem::Get();
    graphicsSystem->SetClearColor(Colors::Gray);
    mInputValues.push_back(mValue1);
    mInputValues.push_back(mValue2);
    mOutputValues.push_back(mOutPutValue);
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
    auto inputSystem = InputSystem::Get();
    if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
    {
        WallG::MainApp().Quit();
        return;
    }


}

void GameState::DebugUI()
{
    if (ImGui::Button("Train"))
    {
        mAppLog.AddLog("Training \n");
        for (int i = 0; i < mTrainTimes; ++i)
        {
            mNerualNextwork.FeedFoward(mInputValues);
            mNerualNextwork.BackPropagate(mOutputValues);
        }
    }
    if (ImGui::Button("Show"))
    {
        auto result = mNerualNextwork.GetResults();
        for (size_t i = 0; i < result.size(); i++)
        {
            mAppLog.AddLog("%f \n" , result[i]);
        }
    }
    if (ImGui::Button("Clean"))
    {
        mAppLog.AddLog("Clean \n");
        mInputValues.clear();
        mOutputValues.clear();
        mInputValues.push_back(mValue1);
        mInputValues.push_back(mValue2);
        mOutputValues.push_back(mOutPutValue);
    }
    ImGui::DragFloat("value1:", &mValue1, 1.0f, 0.0f, 1.0f);
    ImGui::DragFloat("value2:", &mValue2, 1.0f, 0.0f, 1.0f);
    ImGui::DragFloat("OutPutValue:", &mOutPutValue, 1.0f, 0.0f, 1.0f);
    mAppLog.Draw("XOR");
}



