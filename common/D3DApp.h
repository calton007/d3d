#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>

class D3DApp {
public:
    D3DApp(HINSTANCE instance, const wchar_t* title);
    virtual ~D3DApp();

    int Run();

protected:
    virtual bool OnCreate() = 0;
    virtual void OnResize() {}
    virtual void OnUpdate(float deltaSeconds) = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() {}
    virtual void OnKeyDown(WPARAM key) {}

    bool InitializeWindow();
    bool InitializeD3D();
    bool ResizeBackBuffer(UINT width, UINT height);
    bool CompileShader(
        const std::wstring& relativePath,
        const char* entryPoint,
        const char* target,
        Microsoft::WRL::ComPtr<ID3DBlob>& bytecode);
    std::wstring FindAsset(const std::wstring& relativePath) const;
    void Fail(const std::wstring& message) const;

    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HINSTANCE instance_;
    HWND hwnd_ = nullptr;
    std::wstring title_;
    UINT width_ = 640;
    UINT height_ = 480;

    Microsoft::WRL::ComPtr<ID3D11Device> device_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer_;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthView_;
};
