#pragma once

namespace WallG::Graphics
{
	class DepthStencilState final
	{
	public:
		static void ClearState();

	public:
		DepthStencilState() = default;
		~DepthStencilState();

		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		void Initialize(bool depthEnable, bool depthWrite);
		void Terminate();

		void Set();

	private:
		ID3D11DepthStencilState* mDepthStencilState = nullptr;
	};
}