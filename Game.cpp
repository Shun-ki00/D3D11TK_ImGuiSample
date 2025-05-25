//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include <WICTextureLoader.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
    :
    m_text{u8"�n���[���[���h"}
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */


    // ImGui�̏�����========================================================

       //  �o�[�W�����̊m�F
    IMGUI_CHECKVERSION();
    //  �R���e�L�X�g�̍쐬
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // �L�[�{�[�h�ɂ��i�r�Q�[�V�����̗L����
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // �R���g���[���[�ɂ��i�r�Q�[�V�����̗L����
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // �}���`�r���[�|�[�g�L����
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // �h�b�L���O�L����
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // �L�[�{�[�h�i�r�Q�[�V�������Ƀ}�E�X�J�[�\���̈ʒu�������ňړ�����
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    // �^�b�`�X�N���[���Ή��i���͂��^�b�`����O��ɐ؂�ւ��j
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
    // �F�̈�����sRGB��ԂƂ��ă����_�����O����i��ɃJ�X�^�������_���p�j
    io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;
    
    // ���{��Ή�
    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 20.0f, &config, io.Fonts->GetGlyphRangesJapanese());

    //  Win32�p�̏�����
    ImGui_ImplWin32_Init(window);
    //  DirectX11�p�̏�����
    ID3D11Device* device = m_deviceResources->GetD3DDevice();
    ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();
    ImGui_ImplDX11_Init(device, context);

    // ����������
    m_chackBox = false;
    m_float = 0.0f;
    m_counter = 0;
    m_color[0] = 0.0f;
    m_color[1] = 0.0f;
    m_color[2] = 0.0f;

    // �e�N�X�`�������[�h����
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        device,
        L"Resources/DirectX11.jpg",
        nullptr,
        m_texture.ReleaseAndGetAddressOf()
    );
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    //  �V�t���[���̊J�n
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigViewportsNoTaskBarIcon = false; // OS�̃^�X�N�o�[�ɃA�C�R����\�����Ȃ�
    io.ConfigViewportsNoDecoration = false;  // OS�E�B���h�E�̘g������

    //  �f���E�B���h�E�̕`��
    ImGui::ShowDemoWindow();

    // TODO: Add your rendering code here.
    context;

    // �E�B���h�E�̐F��ύX
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBg] =
        ImVec4(m_color[0],m_color[1],m_color[2], 1.0f); // RGBA (0.0�`1.0)

    ImGui::Begin("TextWindow");

    // �e�L�X�g�\��
    ImGui::Text("Hello, world!");  
    ImGui::Text(u8"�n���[���[���h");

    // �`�F�b�N�{�b�N�X
    ImGui::Checkbox("CheckBox", &m_chackBox);

    // ���l float�^
    ImGui::SliderFloat("SliderFloat", &m_float, 0.0f, 10.0f);
    ImGui::DragFloat("Dragfloat", &m_float, 0.0f, 10.0f);
    ImGui::InputFloat("Inputfloat", &m_float, 0.0f, 10.0f);

    // �{�^���@�{�@�J�E���^
    if (ImGui::Button("Button")) m_counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", m_counter);

    // �e�L�X�g����
    ImGui::InputText("Input", m_text,20);

    // �J���[�s�b�J�[
    ImGui::ColorEdit3("clear color", (float*)&m_color);

    ImGui::Image((ImTextureID)m_texture.Get(), ImVec2(200, 200));

    // ������
    ImGui::Separator(); 

    // FPS
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    //  ImGui�̕`�揈��
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // �}���`�r���[�|�[�g�p�ǉ�����
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
