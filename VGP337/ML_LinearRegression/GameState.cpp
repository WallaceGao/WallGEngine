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

    mApplog.AddLog("Generating data ....\n");
    mDataset = Datasets::MakeLinear( 100, 100.0f, -0.5f, 0.0f, 100.0f, 10.0f);
    mLinReg.learningRate = 0.00045f;

    
    for (int e = 0; e < epoch; ++e)
    {
        float error = mLinReg.Fit(mDataset);
        mApplog.AddLog("b0 = %f, b1 - %f , error = %f\n", mLinReg.b0, mLinReg.b1, error);
    }
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

    if (inputSystem->IsKeyPressed(KeyCode::SPACE))
    {
        mApplog.AddLog("Bam! \n");
    }
}

void GameState::DebugUI()
{
    if (ImPlot::BeginPlot("Linear Regression", "x", "Y"))
    {
        float startX = 0.0f;
        float endX = 100.0f;
        float startY = mLinReg.Predict(startX);
        float endY = mLinReg.Predict(endX);
        std::vector<ImVec2> line;
        line.push_back({ startX, startY });
        line.push_back({ endX, endY });
    
        ImPlot::PlotScatter("Dataset", mDataset.x0.data(), mDataset.Y.data(), mDataset.x0.size());
        ImPlot::PlotLine("Model", line.data(), line.size());
        ImPlot::EndPlot();
    }
    mApplog.Draw("Console");

    if (ImGui::Button("try"))
    {
        for (int e = 0; e < epoch; ++e)
        {
            float error = mLinReg.Fit(mDataset);
            mApplog.AddLog("b0 = %f, b1 - %f , error = %f\n", mLinReg.b0, mLinReg.b1, error);
        }
    }
}
