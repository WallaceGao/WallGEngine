#include "Precompiled.h"
#include "PreceptionModule.h"

using namespace WallG::AI;
using namespace WallG::Core;

PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator calculator):
	mAgent(agent), ComputeImportance(std::move(calculator))
{
}

void PerceptionModule::Update(float deltaTime)
{
	//Update sensors and refresh/ add memory records
	for (auto& sensor : mSensors)
	{
		sensor->Update(mAgent, mMemory, deltaTime);
	}
	//Remove any records that are too old
	for (auto iter = mMemory.begin(); iter != mMemory.end();)
	{
		if (iter->lastRecordedTime + mMemorySpan < TimeUtil::GetTime())
			iter = mMemory.erase(iter);
		else
			++iter;
	}

	//Calculate importance on remaining records
	for (auto& record : mMemory)
	{
		record.importance = ComputeImportance(mAgent, record);
	}

	//Sort record by importance
	mMemory.sort([](const auto& a, const auto& b)
	{
		return a.importance > b.importance;
	});
}
