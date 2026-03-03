#include "D3D12ImGui.h"

ZenthEngine::D3D12ImGui::D3D12ImGui() : m_wnd(*dynamic_cast<WindowsWindow*>(Get().Window.get())), m_gfx(*dynamic_cast<D3D12Graphics*>(Get().Graphics.get()))
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(m_wnd.GetWindowHandle());

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
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
}

void ZenthEngine::D3D12ImGui::Draw()
{
    ImGui::Render();
    
    m_gfx.GetCommandList()->SetDescriptorHeaps(1, &m_imguiDescHeap);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_gfx.GetCommandList());
}
