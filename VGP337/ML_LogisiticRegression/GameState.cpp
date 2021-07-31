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


    mLogReg.learningRate = learningRate;

    GenerateData();
    Train();
    //for (int e = 0; e < epoch; ++e)
    //{
    //    float error = mLogReg.Fit(mDataset);
    //    mApplog.AddLog("b0 = %f, b1 - %f , error = %f\n", mLogReg.b0, mLogReg.b1, error);
    //}

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
    if (ImPlot::BeginPlot("Logistic Regression", "x0", "x1"))
    {
        const float m = -mLogReg.b1 / mLogReg.b2;
        const float b = -mLogReg.b0 / mLogReg.b2;
        const float startX = minX;
        const float endX = maxX;
        const float startY = startX * m + b;
        const float endY = endX * m + b;
        std::vector<ImVec2> modelLine;

        modelLine.push_back({ startX, startY });
        modelLine.push_back({ endX, endY });
        ImPlot::PlotLine("Model", modelLine.data(), (int)modelLine.size());

        std::vector<float> ax, ay, bx, by;
        for (int i = 0; i < mDataset.x0.size(); ++i)
        {
            float p = mLogReg.b0 + mLogReg.b1 * mDataset.x0[i] + mLogReg.b2 * mDataset.x1[i];
            if (p >= 0)
            {
                ax.push_back(mDataset.x0[i]);
                ay.push_back(mDataset.x1[i]);
            }
            else
            {
                bx.push_back(mDataset.x0[i]);
                by.push_back(mDataset.x1[i]);
            }
        }
        if (!ax.empty())
            ImPlot::PlotScatter("Ones", ax.data(), ay.data(), (int)ax.size());
        if (!bx.empty())
            ImPlot::PlotScatter("Zeros", bx.data(), by.data(), (int)bx.size());
        ImPlot::EndPlot();
    }
    if (ImGui::Button("try"))
    {
        GenerateData();
    }
    if (ImGui::Button("try"))
    {
        Train();
    }
       
    mApplog.Draw("Console");

    ImGui::DragFloat("learningRate", (float*)&learningRate, 0.00001f, 0.0f, 1.0f, "%.6f");
    ImGui::DragFloat("b0", (float*)&b0, 0.01f);
    ImGui::DragFloat("b1", (float*)&b1, 0.01f);
    ImGui::DragFloat("b2", (float*)&b2, 0.01f);
    ImGui::DragInt("epoch", (int*)&epoch, 1);
}

void GameState::GenerateData()
{
    mApplog.AddLog("Generating data ...\n");
    mDataset = Datasets::MakeLogistic(100, b0, b1, b2, minX, maxX, noise);
}

void GameState::Train()
{
    for (int i = 0; i < 100; ++i)
    {
        float error = mLogReg.Fit(mDataset);
        mApplog.AddLog("b0 = %f, b1 = %f, b2 = %f, error = %f\n", mLogReg.b0, mLogReg.b1, mLogReg.b2, error);
    }
}
