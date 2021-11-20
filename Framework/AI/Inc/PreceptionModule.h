#pragma once

#include "MemoryRecord.h"
#include "Sensor.h"

namespace WallG::AI
{
	class Agent;

	using ImportanceCalculator = std::function<float(const Agent&, MemoryRecord&)>;

	class PerceptionModule
	{
	public:
		PerceptionModule(Agent& agent, ImportanceCalculator calculator);

		template<class SensorType>
		SensorType* AddSensor();

		void Update(float deltaTime);

		void SetMemorySpan(float memorySpan) { mMemorySpan = memorySpan; }
		const MemoryRecords& GetMemoryRecords() const { return mMemory; }
	private:
		using Sensors = std::vector<std::unique_ptr<Sensor>>;

		Agent& mAgent;
		ImportanceCalculator ComputeImportance;
		Sensors mSensors;
		MemoryRecords mMemory;
		float mMemorySpan = 0.0f;
	};

	template <class SensorType>
	SensorType* PerceptionModule::AddSensor()
	{
		static_assert(std::is_base_of_v<Sensor, SensorType>, "SensorType is not a child class of AI::Sensor");
		auto& newSensor = mSensors.emplace_back(std::make_unique<SensorType>());
		return static_cast<SensorType*>(newSensor.get());
	}
}
