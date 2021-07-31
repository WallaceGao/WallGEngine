#pragma once

namespace WallG::Core
{
	class WindowMessageHandler
	{
	public:
		//function pointer
		using Callback = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

		void Hook(HWND window, Callback cb);
		void Unhook();

		LRESULT ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND mWindow = nullptr;
		Callback mPreviousCallback = nullptr;
	};
}
