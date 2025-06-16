# Dear ImGui for DirectX11 サンプルプログラム

## 概要

このプロジェクトは、**DirectX 11 + Dear ImGui** を使用した GUI 描画のサンプルです。  
Dear ImGui の導入・描画手順を最小構成で確認できるよう設計されています。

基本的なウィジェット（ボタン、スライダー、カラーピッカーなど）を表示し、**リアルタイムUI描画の流れ**を理解できます。

## 特徴

- **Dear ImGui** を DirectX 11 環境へ組み込み済み
- 基本ウィジェット、フレーム時間表示、デモウィンドウ搭載
- 背景色の動的変更、ボタン操作などの例を含む
- FPS 表示付きでパフォーマンス確認も可能

## スクリーンショット

![スクリーンショット 2025-06-02 122510](https://github.com/user-attachments/assets/b9dbf4d0-e947-495b-b015-6223f92117d1)

- 左：自作ImGuiウィンドウ（日本語テキスト、スライダー、色選択など）
- 右：ImGui公式のデモウィンドウ

## ✅ 導入時の注意点（重要）

ImGui を Visual Studio プロジェクトに導入する際、以下の設定を **必ず確認してください**。

---

### ImGui の導入に必要な設定（初期化処理 + ビルド設定）

#### 📌 1. 初期化処理はそのままコピーで動作します

`Main.cpp` および `Game.cpp` に記述されている ImGui 初期化コードは、**基本的にそのままコピー＆ペーストで動作します**。

---

#### 📌 2. ImGui の `.cpp` ファイルには「プリコンパイル済みヘッダーを使用しない」設定が必須です

この設定を行わないと、ビルドエラーが発生します。

**対象ファイル一覧：**
- `imgui.cpp`  
- `imgui_draw.cpp`  
- `imgui_tables.cpp`  
- `imgui_widgets.cpp`  
- `imgui_impl_dx11.cpp`  
- `imgui_impl_win32.cpp`

## 動作環境
・OS：Windows 10 / 11  
・GPU : Direct3D 11対応グラフィックカード  
・開発環境：Visual Studio 2022  
・ビルド対象：x64  

## 使用ライブラリ
・Direct3D 11  
・DirectXTK  
・Windows SDK  
・Dear ImGui  

