#pragma once

namespace WallG::Core
{
    class Window
    {
    public:
        Window() = default;

        void Initialize(HINSTANCE instance, std::wstring appName, uint32_t width, uint32_t height, bool maximize = false);
        void Terminate();
        
        void ProcessMessage();

        HWND GetWindowHandle() const { return mWindow; }
        bool IsActive() const { return mActive; }

    private:
        HINSTANCE mInstance = nullptr;
        HWND mWindow = nullptr;
        std::wstring mAppName;
        bool mActive = false;
    };
}
