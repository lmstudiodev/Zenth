#include "D3D12ImGui.h"

ZenthEngine::D3D12ImGui::D3D12ImGui(WindowsWindow& wnd, D3D12Graphics& gfx) : m_wnd(wnd), m_gfx(gfx)
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // === Win32 ===

    ImGui_ImplWin32_Init(m_wnd.GetWindowHandle());

    // === D3D12 Init ===

    D3D12_DESCRIPTOR_HEAP_DESC imguiDescHepDesc{};
    imguiDescHepDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    imguiDescHepDesc.NumDescriptors = 1;
    imguiDescHepDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    imguiDescHepDesc.NodeMask = 0;

    m_gfx.GetDevice()->CreateDescriptorHeap(&imguiDescHepDesc, IID_PPV_ARGS(&m_imguiDescHeap));

    ImGui_ImplDX12_Init(
        m_gfx.GetDevice(),
        D3D12Graphics::BufferCount,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        m_imguiDescHeap,
        m_imguiDescHeap->GetCPUDescriptorHandleForHeapStart(),
        m_imguiDescHeap->GetGPUDescriptorHandleForHeapStart()
    );
}

ZenthEngine::D3D12ImGui::~D3D12ImGui()
{
    ImGui_ImplDX12_Shutdown();
    m_imguiDescHeap.Release();

    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}

void ZenthEngine::D3D12ImGui::NewFrame()
{

}

void ZenthEngine::D3D12ImGui::Draw()
{

}
