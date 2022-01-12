#include "Precompiled.h"
#include "TransformComponent.h"

using namespace WallG;

MEMPOOL_DEFINE(TransformComponent, 1000);

void WallG::TransformComponent::DebugUI()
{
    ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);

    SimpleDraw::AddLine(mPosition, mPosition+ Math::Vector3::XAxis, Colors::Red);
    SimpleDraw::AddLine(mPosition, mPosition+ Math::Vector3::YAxis, Colors::Green);
    SimpleDraw::AddLine(mPosition, mPosition+ Math::Vector3::ZAxis, Colors::Blue);
}
