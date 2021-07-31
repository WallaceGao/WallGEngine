#pragma once

namespace WallG
{
	class AppState;

	struct AppConfig
	{
		std::wstring appName = L"WallG Game Engine";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
		bool escToQuit = true;
	};
	class App
	{
	public:

		template<class StateType>
		void AddState(std::string name)
		{
			auto [iter, result] = mAppState.emplace(std::move(name), std::make_unique<StateType>());
			if (result && mCurrentState == nullptr)
			{
				LOG("App -- Starting State: %s", iter->first.c_str());
				mCurrentState = iter->second.get();
			}
		}

		void ChangeState(const std::string& name);

		void Run(const AppConfig& appConfig);
		void Quit() { mRunning = false; }

	private:
		using AppStates = std::map<std::string, std::unique_ptr<AppState>>;

		AppStates mAppState;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
		bool mRunning = false;
	};
}
