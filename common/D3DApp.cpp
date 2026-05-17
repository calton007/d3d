#include "D3DApp.h"

#include <chrono>
#include <filesystem>
#include <sstream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

namespace {
std::wstring LastErrorMessage(const wchar_t* prefix) {
    DWORD error = GetLastError();
    std::wstringstream ss;
    ss << prefix << L" failed. GetLastError=" << error;
    return ss.str();
}

bool Exists(const std::filesystem::path& path) {
    std::error_code ec;
    return std::filesystem::exists(path, ec);
}
}

D3DApp::D3DApp(HINSTANCE instance, const wchar_t* title)
    : instance_(instance), title_(title) {}

D3DApp::~D3DApp() = default;

int D3DApp::Run() {
    if (!InitializeWindow() || !InitializeD3D() || !OnCreate()) {
        return 1;
    }

    MSG msg{};
    int smokeFrames = 0;
    wchar_t smokeValue[16]{};
    if (GetEnvironmentVariableW(L"D3D_SAMPLE_SMOKE", smokeValue, ARRAYSIZE(smokeValue)) > 0) {
        smokeFrames = 8;
    }

    auto previous = std::chrono::steady_clock::now();
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        auto current = std::chrono::steady_clock::now();
        float delta = std::chrono::duration<float>(current - previous).count();
        previous = current;
        OnUpdate(delta);
        OnRender();
        if (smokeFrames > 0 && --smokeFrames == 0) {
            DestroyWindow(hwnd_);
        }
    }

    OnDestroy();
    return static_cast<int>(msg.wParam);
}

bool D3DApp::InitializeWindow() {
    WNDCLASSW wc{};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = D3DApp::StaticWndProc;
    wc.hInstance = instance_;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"D3D11TeachingSample";

    if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        Fail(LastErrorMessage(L"RegisterClassW"));
        return false;
    }

    RECT rect{0, 0, static_cast<LONG>(width_), static_cast<LONG>(height_)};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd_ = CreateWindowW(
        wc.lpszClassName,
        title_.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        instance_,
        this);

    if (!hwnd_) {
        Fail(LastErrorMessage(L"CreateWindowW"));
        return false;
    }

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
    return true;
}

bool D3DApp::InitializeD3D() {
    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferDesc.Width = width_;
    desc.BufferDesc.Height = height_;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 1;
    desc.OutputWindow = hwnd_;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT flags = 0;

    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    D3D_FEATURE_LEVEL createdLevel{};

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        levels,
        ARRAYSIZE(levels),
        D3D11_SDK_VERSION,
        &desc,
        swapChain_.GetAddressOf(),
        device_.GetAddressOf(),
        &createdLevel,
        context_.GetAddressOf());

    if (FAILED(hr)) {
        std::wstringstream ss;
        ss << L"D3D11CreateDeviceAndSwapChain failed. HRESULT=0x" << std::hex << hr;
        Fail(ss.str());
        return false;
    }

    return ResizeBackBuffer(width_, height_);
}

bool D3DApp::ResizeBackBuffer(UINT width, UINT height) {
    if (!swapChain_) {
        return true;
    }

    width_ = width > 0 ? width : 1;
    height_ = height > 0 ? height : 1;

    context_->OMSetRenderTargets(0, nullptr, nullptr);
    renderTarget_.Reset();
    depthView_.Reset();
    depthBuffer_.Reset();

    HRESULT hr = swapChain_->ResizeBuffers(0, width_, height_, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) {
        std::wstringstream ss;
        ss << L"ResizeBuffers failed. HRESULT=0x" << std::hex << hr;
        Fail(ss.str());
        return false;
    }

    ComPtr<ID3D11Texture2D> backBuffer;
    hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    if (FAILED(hr)) {
        Fail(L"IDXGISwapChain::GetBuffer failed.");
        return false;
    }

    hr = device_->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTarget_.GetAddressOf());
    if (FAILED(hr)) {
        Fail(L"CreateRenderTargetView failed.");
        return false;
    }

    D3D11_TEXTURE2D_DESC depthDesc{};
    depthDesc.Width = width_;
    depthDesc.Height = height_;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = device_->CreateTexture2D(&depthDesc, nullptr, depthBuffer_.GetAddressOf());
    if (FAILED(hr)) {
        Fail(L"CreateTexture2D depth buffer failed.");
        return false;
    }

    hr = device_->CreateDepthStencilView(depthBuffer_.Get(), nullptr, depthView_.GetAddressOf());
    if (FAILED(hr)) {
        Fail(L"CreateDepthStencilView failed.");
        return false;
    }

    D3D11_VIEWPORT viewport{};
    viewport.Width = static_cast<float>(width_);
    viewport.Height = static_cast<float>(height_);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    context_->RSSetViewports(1, &viewport);
    OnResize();
    return true;
}

bool D3DApp::CompileShader(
    const std::wstring& relativePath,
    const char* entryPoint,
    const char* target,
    ComPtr<ID3DBlob>& bytecode) {
    std::wstring path = FindAsset(relativePath);
    if (path.empty()) {
        Fail(L"Missing shader file: " + relativePath);
        return false;
    }

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ComPtr<ID3DBlob> errors;
    HRESULT hr = D3DCompileFromFile(
        path.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        target,
        flags,
        0,
        bytecode.GetAddressOf(),
        errors.GetAddressOf());

    if (FAILED(hr)) {
        std::wstring message = L"Shader compile failed: " + path;
        if (errors) {
            OutputDebugStringA(static_cast<const char*>(errors->GetBufferPointer()));
        }
        Fail(message);
        return false;
    }
    return true;
}

std::wstring D3DApp::FindAsset(const std::wstring& relativePath) const {
    wchar_t moduleName[MAX_PATH]{};
    GetModuleFileNameW(nullptr, moduleName, MAX_PATH);

    std::filesystem::path starts[] = {
        std::filesystem::current_path(),
        std::filesystem::path(moduleName).parent_path(),
    };

    for (auto start : starts) {
        for (int i = 0; i < 8; ++i) {
            auto candidate = start / relativePath;
            if (Exists(candidate)) {
                return candidate.wstring();
            }
            if (!start.has_parent_path() || start == start.parent_path()) {
                break;
            }
            start = start.parent_path();
        }
    }

    return L"";
}

void D3DApp::Fail(const std::wstring& message) const {
    OutputDebugStringW((message + L"\n").c_str());
    MessageBoxW(hwnd_, message.c_str(), title_.c_str(), MB_ICONERROR | MB_OK);
}

LRESULT CALLBACK D3DApp::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    D3DApp* app = nullptr;
    if (msg == WM_NCCREATE) {
        auto create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        app = static_cast<D3DApp*>(create->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    } else {
        app = reinterpret_cast<D3DApp*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (app) {
        return app->WndProc(hwnd, msg, wParam, lParam);
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT D3DApp::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if (device_ && wParam != SIZE_MINIMIZED) {
            ResizeBackBuffer(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
            return 0;
        }
        OnKeyDown(wParam);
        return 0;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}
