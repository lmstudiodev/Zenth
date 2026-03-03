#include "D3D12ImGui.h"

ZenthEngine::D3D12ImGui::D3D12ImGui() : m_wnd(*dynamic_cast<WindowsWindow*>(Get().Window.get())), m_gfx(*dynamic_cast<D3D12Graphics*>(Get().Graphics.get()))
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

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

void ZenthEngine::D3D12ImGui::Draw()
{
    ImGui::Render();

    auto cmdList = m_gfx.SetupAndGetCommandList();
    
    cmdList->SetDescriptorHeaps(1, &m_imguiDescHeap);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);

    m_gfx.DrawCommandList();
}

bool ZenthEngine::D3D12ImGui::Update(float dt)
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    return true;
}
