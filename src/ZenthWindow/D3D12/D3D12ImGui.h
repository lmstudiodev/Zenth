#pragma once
#include <ZenthCore/IImGui.h>
#include <ZenthCore/ZenthContext.h>
#include <ZenthWindow/WinInclude.h>
#include <ZenthWindow/WindowsWindow.h>
#include <ZenthWindow/D3D12/D3D12Graphics.h>
#include <imgui/imgui.h>
#include <Imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>

namespace ZenthEngine
{
	class D3D12ImGui : public IImGui
	{
	public:
		D3D12ImGui(WindowsWindow& wnd, D3D12Graphics& gfx);
		~D3D12ImGui();

		void NewFrame() override;
		void Draw() override;

	private:
		WindowsWindow& m_wnd;
		D3D12Graphics& m_gfx;

		ComPointer<ID3D12DescriptorHeap> m_imguiDescHeap;
	};
}


