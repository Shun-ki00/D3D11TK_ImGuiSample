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
    m_text{u8"ハローワールド"}
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


    // ImGuiの初期化========================================================

       //  バージョンの確認
    IMGUI_CHECKVERSION();
    //  コンテキストの作成
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // キーボードによるナビゲーションの有効化
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // コントローラーによるナビゲーションの有効化
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // マルチビューポート有効化
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // ドッキング有効化
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // キーボードナビゲーション中にマウスカーソルの位置も自動で移動する
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    // タッチスクリーン対応（入力がタッチ環境を前提に切り替わる）
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
    // 色の扱いをsRGB空間としてレンダリングする（主にカスタムレンダラ用）
    io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;
    
    // 日本語対応
    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 20.0f, &config, io.Fonts->GetGlyphRangesJapanese());

    //  Win32用の初期化
    ImGui_ImplWin32_Init(window);
    //  DirectX11用の初期化
    ID3D11Device* device = m_deviceResources->GetD3DDevice();
    ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();
    ImGui_ImplDX11_Init(device, context);

    // 初期化処理
    m_chackBox = false;
    m_float = 0.0f;
    m_counter = 0;
    m_color[0] = 0.0f;
    m_color[1] = 0.0f;
    m_color[2] = 0.0f;

    // テクスチャをロードする
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

    //  新フレームの開始
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigViewportsNoTaskBarIcon = false; // OSのタスクバーにアイコンを表示しない
    io.ConfigViewportsNoDecoration = false;  // OSウィンドウの枠を消す

    //  デモウィンドウの描画
    ImGui::ShowDemoWindow();

    // TODO: Add your rendering code here.
    context;

    // ウィンドウの色を変更
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBg] =
        ImVec4(m_color[0],m_color[1],m_color[2], 1.0f); // RGBA (0.0～1.0)

    ImGui::Begin("TextWindow");

    // テキスト表示
    ImGui::Text("Hello, world!");  
    ImGui::Text(u8"ハローワールド");

    // チェックボックス
    ImGui::Checkbox("CheckBox", &m_chackBox);

    // 数値 float型
    ImGui::SliderFloat("SliderFloat", &m_float, 0.0f, 10.0f);
    ImGui::DragFloat("Dragfloat", &m_float, 0.0f, 10.0f);
    ImGui::InputFloat("Inputfloat", &m_float, 0.0f, 10.0f);

    // ボタン　＋　カウンタ
    if (ImGui::Button("Button")) m_counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", m_counter);

    // テキスト入力
    ImGui::InputText("Input", m_text,20);

    // カラーピッカー
    ImGui::ColorEdit3("clear color", (float*)&m_color);

    ImGui::Image((ImTextureID)m_texture.Get(), ImVec2(200, 200));

    // 水平線
    ImGui::Separator(); 

    // FPS
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    //  ImGuiの描画処理
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // マルチビューポート用追加処理
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
