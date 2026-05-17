#include "SampleScene.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace {
struct Vertex {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
    XMFLOAT4 color;
};

struct Constants {
    XMMATRIX worldViewProjection;
    XMMATRIX world;
    XMFLOAT4 lightDirection;
    XMFLOAT4 tint;
    XMFLOAT4 options;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
};

struct BoxPart {
    XMFLOAT3 center;
    XMFLOAT3 size;
    XMFLOAT4 color;
};

MeshData MakeTriangle() {
    return {
        {
            {{-1.2f, -0.9f, 0.0f}, {0, 0, -1}, {0, 1}, {0.1f, 0.8f, 0.2f, 1}},
            {{0.0f, 1.1f, 0.0f}, {0, 0, -1}, {0.5f, 0}, {1.0f, 0.1f, 0.1f, 1}},
            {{1.2f, -0.9f, 0.0f}, {0, 0, -1}, {1, 1}, {0.1f, 0.25f, 1.0f, 1}},
        },
        {0, 1, 2},
    };
}

void AddFace(std::vector<Vertex>& v, std::vector<uint16_t>& i, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, XMFLOAT3 d, XMFLOAT3 n, XMFLOAT4 color) {
    uint16_t base = static_cast<uint16_t>(v.size());
    v.push_back({a, n, {0, 1}, color});
    v.push_back({b, n, {0, 0}, color});
    v.push_back({c, n, {1, 0}, color});
    v.push_back({d, n, {1, 1}, color});
    i.insert(i.end(), {base, static_cast<uint16_t>(base + 1), static_cast<uint16_t>(base + 2), base, static_cast<uint16_t>(base + 2), static_cast<uint16_t>(base + 3)});
}

void AddFaceUv(std::vector<Vertex>& v, std::vector<uint16_t>& i, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, XMFLOAT3 d, XMFLOAT3 n, XMFLOAT4 color, float uScale, float vScale) {
    uint16_t base = static_cast<uint16_t>(v.size());
    v.push_back({a, n, {0, vScale}, color});
    v.push_back({b, n, {0, 0}, color});
    v.push_back({c, n, {uScale, 0}, color});
    v.push_back({d, n, {uScale, vScale}, color});
    i.insert(i.end(), {base, static_cast<uint16_t>(base + 1), static_cast<uint16_t>(base + 2), base, static_cast<uint16_t>(base + 2), static_cast<uint16_t>(base + 3)});
}

void AddTriangle(std::vector<Vertex>& v, std::vector<uint16_t>& i, XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, XMFLOAT4 color) {
    XMVECTOR av = XMLoadFloat3(&a);
    XMVECTOR bv = XMLoadFloat3(&b);
    XMVECTOR cv = XMLoadFloat3(&c);
    XMFLOAT3 normal{};
    XMStoreFloat3(&normal, XMVector3Normalize(XMVector3Cross(bv - av, cv - av)));

    uint16_t base = static_cast<uint16_t>(v.size());
    v.push_back({a, normal, {0.0f, 1.0f}, color});
    v.push_back({b, normal, {0.5f, 0.0f}, color});
    v.push_back({c, normal, {1.0f, 1.0f}, color});
    i.insert(i.end(), {base, static_cast<uint16_t>(base + 1), static_cast<uint16_t>(base + 2)});
}

MeshData MakeCube() {
    std::vector<Vertex> v;
    std::vector<uint16_t> i;
    AddFace(v, i, {-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, {1, -1, -1}, {0, 0, -1}, {0.9f, 0.2f, 0.15f, 1});
    AddFace(v, i, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}, {-1, -1, 1}, {0, 0, 1}, {0.15f, 0.55f, 0.95f, 1});
    AddFace(v, i, {-1, -1, 1}, {-1, 1, 1}, {-1, 1, -1}, {-1, -1, -1}, {-1, 0, 0}, {0.2f, 0.8f, 0.35f, 1});
    AddFace(v, i, {1, -1, -1}, {1, 1, -1}, {1, 1, 1}, {1, -1, 1}, {1, 0, 0}, {1.0f, 0.75f, 0.1f, 1});
    AddFace(v, i, {-1, 1, -1}, {-1, 1, 1}, {1, 1, 1}, {1, 1, -1}, {0, 1, 0}, {0.65f, 0.35f, 1.0f, 1});
    AddFace(v, i, {-1, -1, 1}, {-1, -1, -1}, {1, -1, -1}, {1, -1, 1}, {0, -1, 0}, {0.9f, 0.9f, 0.9f, 1});
    return {v, i};
}

MeshData MakeTexturedCube(float uvScale) {
    std::vector<Vertex> v;
    std::vector<uint16_t> i;
    AddFaceUv(v, i, {-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, {1, -1, -1}, {0, 0, -1}, {1, 1, 1, 1}, uvScale, uvScale);
    AddFaceUv(v, i, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}, {-1, -1, 1}, {0, 0, 1}, {1, 1, 1, 1}, uvScale, uvScale);
    AddFaceUv(v, i, {-1, -1, 1}, {-1, 1, 1}, {-1, 1, -1}, {-1, -1, -1}, {-1, 0, 0}, {1, 1, 1, 1}, uvScale, uvScale);
    AddFaceUv(v, i, {1, -1, -1}, {1, 1, -1}, {1, 1, 1}, {1, -1, 1}, {1, 0, 0}, {1, 1, 1, 1}, uvScale, uvScale);
    AddFaceUv(v, i, {-1, 1, -1}, {-1, 1, 1}, {1, 1, 1}, {1, 1, -1}, {0, 1, 0}, {1, 1, 1, 1}, uvScale, uvScale);
    AddFaceUv(v, i, {-1, -1, 1}, {-1, -1, -1}, {1, -1, -1}, {1, -1, 1}, {0, -1, 0}, {1, 1, 1, 1}, uvScale, uvScale);
    return {v, i};
}

MeshData MakePlane(float y, float size, XMFLOAT4 color) {
    return {
        {
            {{-size, y, -size}, {0, 1, 0}, {0, 1}, color},
            {{-size, y, size}, {0, 1, 0}, {0, 0}, color},
            {{size, y, size}, {0, 1, 0}, {1, 0}, color},
            {{size, y, -size}, {0, 1, 0}, {1, 1}, color},
        },
        {0, 1, 2, 0, 2, 3},
    };
}

MeshData MakeWall(float z, float size, XMFLOAT4 color) {
    return {
        {
            {{-size, -1.0f, z}, {0, 0, -1}, {0, 1}, color},
            {{-size, size, z}, {0, 0, -1}, {0, 0}, color},
            {{size, size, z}, {0, 0, -1}, {1, 0}, color},
            {{size, -1.0f, z}, {0, 0, -1}, {1, 1}, color},
        },
        {0, 1, 2, 0, 2, 3},
    };
}

MeshData MakeTetrahedron(int variant) {
    float lift = variant == 23 ? 1.45f : 1.25f;
    float spread = variant == 23 ? 1.35f : 1.2f;
    return {
        {
            {{0.0f, lift, 0.0f}, {0.0f, 0.7f, -0.7f}, {0.5f, 0.0f}, {0.95f, 0.25f, 0.15f, 1}},
            {{-spread, -0.8f, -0.8f}, {0.0f, 0.7f, -0.7f}, {0.0f, 1.0f}, {0.95f, 0.25f, 0.15f, 1}},
            {{spread, -0.8f, -0.8f}, {0.0f, 0.7f, -0.7f}, {1.0f, 1.0f}, {0.95f, 0.25f, 0.15f, 1}},
            {{0.0f, -0.8f, variant == 23 ? 1.55f : 1.25f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, {0.15f, 0.55f, 0.95f, 1}},
        },
        {0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 3, 2},
    };
}

MeshData MakeOctahedron(int variant) {
    float top = variant == 20 ? 1.65f : 1.4f;
    float side = variant == 20 ? 1.3f : 1.1f;
    return {
        {
            {{0, top, 0}, {0, 1, 0}, {0.5f, 0}, {0.95f, 0.2f, 0.2f, 1}},
            {{-side, 0, 0}, {-1, 0, 0}, {0, 0.5f}, {0.2f, 0.9f, 0.35f, 1}},
            {{0, 0, -side}, {0, 0, -1}, {0.5f, 1}, {0.2f, 0.45f, 1.0f, 1}},
            {{side, 0, 0}, {1, 0, 0}, {1, 0.5f}, {0.95f, 0.85f, 0.15f, 1}},
            {{0, 0, side}, {0, 0, 1}, {0.5f, 0}, {0.8f, 0.35f, 1.0f, 1}},
            {{0, -top, 0}, {0, -1, 0}, {0.5f, 1}, {0.9f, 0.9f, 0.9f, 1}},
        },
        {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1, 5, 2, 1, 5, 3, 2, 5, 4, 3, 5, 1, 4},
    };
}

MeshData MakeArrowhead(int variant) {
    std::vector<Vertex> v;
    std::vector<uint16_t> i;

    float xOffset = variant == 1 ? 0.65f : 0.0f;
    float yOffset = variant == 0 ? -0.35f : 0.0f;
    XMFLOAT3 p0{xOffset, 0.65f + yOffset, 0.0f};
    XMFLOAT3 p1{xOffset - 0.8f, -0.65f + yOffset, -0.8f};
    XMFLOAT3 p2{xOffset, -0.65f + yOffset, 0.8f};
    XMFLOAT3 p3{xOffset, -0.65f + yOffset, -0.8f};
    XMFLOAT3 p4{xOffset + 0.8f, -0.65f + yOffset, -0.8f};

    AddTriangle(v, i, p1, p2, p0, {0.95f, 0.18f, 0.08f, 1.0f});
    AddTriangle(v, i, p0, p2, p1, {0.55f, 0.08f, 0.03f, 1.0f});
    AddTriangle(v, i, p0, p2, p4, {1.0f, 0.65f, 0.05f, 1.0f});
    AddTriangle(v, i, p4, p2, p0, {0.95f, 0.35f, 0.02f, 1.0f});
    AddTriangle(v, i, p0, p2, p3, {0.2f, 0.35f, 0.95f, 1.0f});
    AddTriangle(v, i, p3, p2, p0, {0.08f, 0.12f, 0.35f, 1.0f});

    return {v, i};
}

void AddBox(MeshData& result, XMFLOAT3 center, XMFLOAT3 size, XMFLOAT4 color) {
    MeshData box = MakeCube();
    uint16_t base = static_cast<uint16_t>(result.vertices.size());
    for (auto vertex : box.vertices) {
        vertex.position.x = center.x + vertex.position.x * size.x;
        vertex.position.y = center.y + vertex.position.y * size.y;
        vertex.position.z = center.z + vertex.position.z * size.z;
        vertex.color = color;
        result.vertices.push_back(vertex);
    }
    for (auto index : box.indices) {
        result.indices.push_back(static_cast<uint16_t>(base + index));
    }
}

MeshData MakeChair(int variant) {
    MeshData result;
    XMFLOAT4 wood{0.55f, 0.35f, 0.18f, 1};
    XMFLOAT4 back{0.45f, 0.28f, 0.14f, 1};
    XMFLOAT4 dark{0.25f, 0.16f, 0.09f, 1};
    XMFLOAT4 cushion{0.15f, 0.35f, 0.75f, 1};

    std::vector<BoxPart> parts;
    if (variant == 3) {
        parts = {{{0, 0, 0}, {1.15f, 0.14f, 0.95f}, wood}, {{0, 1.0f, 0.78f}, {1.15f, 0.95f, 0.14f}, back}, {{-0.82f, -0.85f, -0.62f}, {0.13f, 0.85f, 0.13f}, dark}, {{0.82f, -0.85f, -0.62f}, {0.13f, 0.85f, 0.13f}, dark}, {{-0.82f, -0.85f, 0.62f}, {0.13f, 0.85f, 0.13f}, dark}, {{0.82f, -0.85f, 0.62f}, {0.13f, 0.85f, 0.13f}, dark}};
    } else if (variant == 4) {
        parts = {{{0, 0, 0}, {1.35f, 0.16f, 1.05f}, cushion}, {{0, 1.15f, 0.85f}, {1.35f, 1.05f, 0.16f}, back}, {{-0.95f, -0.9f, -0.7f}, {0.15f, 0.9f, 0.15f}, dark}, {{0.95f, -0.9f, -0.7f}, {0.15f, 0.9f, 0.15f}, dark}, {{-0.95f, -0.9f, 0.7f}, {0.15f, 0.9f, 0.15f}, dark}, {{0.95f, -0.9f, 0.7f}, {0.15f, 0.9f, 0.15f}, dark}, {{0, 0.45f, 0.88f}, {1.2f, 0.08f, 0.12f}, dark}};
    } else if (variant == 5) {
        parts = {{{0, 0, 0}, {1.2f, 0.14f, 1.0f}, wood}, {{0, 1.0f, 0.85f}, {1.2f, 0.18f, 0.14f}, back}, {{0, 1.55f, 0.85f}, {1.2f, 0.18f, 0.14f}, back}, {{-0.9f, 0.8f, 0.85f}, {0.13f, 1.1f, 0.13f}, dark}, {{0.9f, 0.8f, 0.85f}, {0.13f, 1.1f, 0.13f}, dark}, {{-0.9f, -0.9f, -0.65f}, {0.14f, 0.9f, 0.14f}, dark}, {{0.9f, -0.9f, -0.65f}, {0.14f, 0.9f, 0.14f}, dark}, {{-0.9f, -0.9f, 0.65f}, {0.14f, 0.9f, 0.14f}, dark}, {{0.9f, -0.9f, 0.65f}, {0.14f, 0.9f, 0.14f}, dark}};
    } else {
        parts = {{{0, 0, 0}, {1.25f, 0.16f, 1.0f}, wood}, {{0, 1.1f, 0.85f}, {1.25f, 1.05f, 0.16f}, back}, {{-0.9f, -0.95f, -0.65f}, {0.16f, 0.95f, 0.16f}, dark}, {{0.9f, -0.95f, -0.65f}, {0.16f, 0.95f, 0.16f}, dark}, {{-0.9f, -0.95f, 0.65f}, {0.16f, 0.95f, 0.16f}, dark}, {{0.9f, -0.95f, 0.65f}, {0.16f, 0.95f, 0.16f}, dark}, {{0, 0.22f, 0}, {1.05f, 0.08f, 0.82f}, cushion}};
    }

    for (const auto& part : parts) {
        AddBox(result, part.center, part.size, part.color);
    }
    return result;
}

class TeachingSampleApp final : public D3DApp {
public:
    TeachingSampleApp(HINSTANCE instance, const wchar_t* title, SampleKind kind, int variant)
        : D3DApp(instance, title), kind_(kind), variant_(variant) {}

private:
    bool OnCreate() override {
        if (!CreatePipeline() || !CreateStates() || !CreateMeshes()) {
            return false;
        }
        if (kind_ == SampleKind::TexturedCube || kind_ == SampleKind::MirrorShadow) {
            std::wstring texture = kind_ == SampleKind::MirrorShadow ? L"assets\\brick.bmp" : TextureName();
            if (!LoadBmpTexture(texture)) {
                return false;
            }
        }
        OnResize();
        return true;
    }

    void OnResize() override {
        float aspect = height_ == 0 ? 1.0f : static_cast<float>(width_) / static_cast<float>(height_);
        projection_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspect, 0.1f, 100.0f);
    }

    void OnUpdate(float deltaSeconds) override {
        rotation_ += deltaSeconds * (0.45f + 0.05f * static_cast<float>(variant_ % 9));
        if (rotation_ > XM_2PI) {
            rotation_ -= XM_2PI;
        }
    }

    void OnRender() override {
        const float clear[] = {0.08f, 0.10f, 0.12f, 1.0f};
        context_->ClearRenderTargetView(renderTarget_.Get(), clear);
        context_->ClearDepthStencilView(depthView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        context_->OMSetRenderTargets(1, renderTarget_.GetAddressOf(), depthView_.Get());

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        context_->IASetInputLayout(inputLayout_.Get());
        context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context_->VSSetShader(vertexShader_.Get(), nullptr, 0);
        context_->PSSetShader(pixelShader_.Get(), nullptr, 0);
        context_->VSSetConstantBuffers(0, 1, constantBuffer_.GetAddressOf());
        context_->PSSetConstantBuffers(0, 1, constantBuffer_.GetAddressOf());
        context_->PSSetSamplers(0, 1, sampler_.GetAddressOf());

        if (kind_ == SampleKind::Triangle) {
            DrawMesh(triangleVB_.Get(), triangleIB_.Get(), 3, XMMatrixRotationY(rotation_), false, {1, 1, 1, 1});
        } else if (kind_ == SampleKind::MirrorShadow) {
            RenderMirrorShadow();
        } else if (kind_ == SampleKind::Arrowhead || kind_ == SampleKind::Chair || kind_ == SampleKind::Octahedron || kind_ == SampleKind::Tetrahedron) {
            DrawMesh(objectVB_.Get(), objectIB_.Get(), objectIndexCount_, XMMatrixRotationY(rotation_) * XMMatrixRotationX(rotation_ * 0.25f), false, {1, 1, 1, 1});
        } else if (kind_ == SampleKind::TexturedCube) {
            RenderTexturedCube();
        } else {
            RenderCubeLike();
        }

        swapChain_->Present(1, 0);
    }

    void OnKeyDown(WPARAM key) override {
        if (key == VK_LEFT) {
            textureOffset_ = 0;
            cameraAngle_ -= 0.18f;
        } else if (key == VK_RIGHT) {
            textureOffset_ = 1;
            cameraAngle_ += 0.18f;
        } else if (key == VK_UP) {
            textureOffset_ = 2;
            cameraHeight_ += 0.25f;
        } else if (key == VK_DOWN) {
            textureOffset_ = 3;
            cameraHeight_ -= 0.25f;
        } else if (key == 'A') {
            cameraAngle_ -= 0.18f;
        } else if (key == 'S') {
            cameraAngle_ += 0.18f;
        }

        if (kind_ == SampleKind::TexturedCube) {
            LoadBmpTexture(TextureName());
        }
    }

    void RenderCubeLike() {
        if (variant_ == 18 || variant_ == 19) {
            DrawMesh(planeVB_.Get(), planeIB_.Get(), 6, XMMatrixIdentity(), false, {0.28f, 0.30f, 0.35f, 1});
            DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, XMMatrixRotationY(rotation_) * XMMatrixTranslation(-1.4f, 0.25f, 0), false, {1, 1, 1, 1});
            DrawMesh(objectVB_.Get(), objectIB_.Get(), objectIndexCount_, XMMatrixRotationY(-rotation_ * 0.6f) * XMMatrixTranslation(1.4f, 0.35f, 0), false, {1, 1, 1, 1});
            return;
        }

        if (variant_ == 11) {
            DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, XMMatrixRotationY(rotation_) * XMMatrixTranslation(-1.25f, 0, 0), false, {1, 1, 1, 1});
            DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, XMMatrixScaling(0.6f, 0.6f, 0.6f) * XMMatrixRotationY(-rotation_) * XMMatrixTranslation(1.25f, 0, 0), false, {0.75f, 0.95f, 1.0f, 1});
            return;
        }

        XMMATRIX world = XMMatrixRotationY(rotation_) * XMMatrixRotationX(rotation_ * 0.45f);
        if (variant_ == 8) {
            world = XMMatrixScaling(1.0f, 1.0f, 1.6f) * XMMatrixTranslation(0, 0, 0.6f) * XMMatrixRotationY(rotation_);
        } else if (variant_ == 9) {
            world = XMMatrixRotationX(XM_PIDIV4) * XMMatrixRotationY(rotation_);
        } else if (variant_ == 10) {
            world = XMMatrixScaling(1.35f, 0.75f, 1.0f) * XMMatrixRotationY(rotation_) * XMMatrixRotationZ(0.35f);
        }
        DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, world, false, {1, 1, 1, 1});
    }

    void RenderTexturedCube() {
        if (variant_ == 25) {
            DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, XMMatrixRotationY(rotation_), true, {1, 1, 1, 1});
        } else if (variant_ == 26) {
            DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, XMMatrixScaling(1.0f, 1.0f, 1.6f) * XMMatrixRotationY(rotation_), true, {1, 1, 1, 1});
        } else {
            DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, XMMatrixRotationY(rotation_) * XMMatrixTranslation(-1.3f, 0, 0), true, {1, 1, 1, 1});
            DrawMesh(objectVB_.Get(), objectIB_.Get(), objectIndexCount_, XMMatrixRotationY(-rotation_) * XMMatrixTranslation(1.3f, 0, 0), true, {0.85f, 1.0f, 0.85f, 1});
        }
    }

    void RenderMirrorShadow() {
        DrawMesh(planeVB_.Get(), planeIB_.Get(), 6, XMMatrixIdentity(), true, {0.9f, 0.9f, 0.9f, 1});
        DrawMesh(wallVB_.Get(), wallIB_.Get(), 6, XMMatrixIdentity(), false, {0.55f, 0.68f, 0.9f, 1});

        XMMATRIX chairWorld = XMMatrixScaling(0.65f, 0.65f, 0.65f) * XMMatrixRotationY(rotation_) * XMMatrixTranslation(-1.4f, 0.0f, -0.8f);
        DrawMesh(chairVB_.Get(), chairIB_.Get(), chairIndexCount_, chairWorld, false, {1, 1, 1, 1});
        DrawMesh(cubeVB_.Get(), cubeIB_.Get(), 36, XMMatrixScaling(0.7f, 0.7f, 0.7f) * XMMatrixRotationY(-rotation_) * XMMatrixTranslation(1.2f, -0.25f, -0.5f), false, {1, 1, 1, 1});

        context_->OMSetDepthStencilState(stencilWriteState_.Get(), 1);
        context_->OMSetBlendState(colorWriteOffBlend_.Get(), nullptr, 0xffffffff);
        DrawMesh(wallVB_.Get(), wallIB_.Get(), 6, XMMatrixIdentity(), false, {1, 1, 1, 1});

        context_->OMSetDepthStencilState(stencilTestState_.Get(), 1);
        context_->OMSetBlendState(alphaBlendState_.Get(), nullptr, 0xffffffff);
        XMMATRIX reflect = XMMatrixReflect(XMVectorSet(0, 0, 1, 2.8f));
        DrawMesh(chairVB_.Get(), chairIB_.Get(), chairIndexCount_, chairWorld * reflect, false, {0.55f, 0.75f, 1.0f, 0.65f});

        context_->OMSetDepthStencilState(nullptr, 0);
        XMMATRIX shadow = XMMatrixScaling(1.0f, 0.03f, 1.0f) * XMMatrixRotationY(rotation_) * XMMatrixTranslation(-1.4f, -0.96f, -0.8f);
        DrawMesh(chairVB_.Get(), chairIB_.Get(), chairIndexCount_, shadow, false, {0.02f, 0.02f, 0.02f, 0.45f});
        context_->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    }

    bool CreatePipeline() {
        ComPtr<ID3DBlob> vs;
        ComPtr<ID3DBlob> ps;
        if (!CompileShader(L"assets\\basic.hlsl", "VSMain", "vs_5_0", vs) ||
            !CompileShader(L"assets\\basic.hlsl", "PSMain", "ps_5_0", ps)) {
            return false;
        }

        HRESULT hr = device_->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, vertexShader_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateVertexShader failed.");
            return false;
        }
        hr = device_->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), nullptr, pixelShader_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreatePixelShader failed.");
            return false;
        }

        D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        hr = device_->CreateInputLayout(layout, ARRAYSIZE(layout), vs->GetBufferPointer(), vs->GetBufferSize(), inputLayout_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateInputLayout failed.");
            return false;
        }

        D3D11_BUFFER_DESC cb{};
        cb.ByteWidth = sizeof(Constants);
        cb.Usage = D3D11_USAGE_DEFAULT;
        cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        hr = device_->CreateBuffer(&cb, nullptr, constantBuffer_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateBuffer constant buffer failed.");
            return false;
        }

        D3D11_SAMPLER_DESC sampler{};
        sampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler.MaxLOD = D3D11_FLOAT32_MAX;
        hr = device_->CreateSamplerState(&sampler, sampler_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateSamplerState failed.");
            return false;
        }
        return true;
    }

    bool CreateStates() {
        D3D11_BLEND_DESC alpha{};
        alpha.RenderTarget[0].BlendEnable = TRUE;
        alpha.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        alpha.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        alpha.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        alpha.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        alpha.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        alpha.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        alpha.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        HRESULT hr = device_->CreateBlendState(&alpha, alphaBlendState_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateBlendState alpha failed.");
            return false;
        }

        D3D11_BLEND_DESC off{};
        off.RenderTarget[0].RenderTargetWriteMask = 0;
        hr = device_->CreateBlendState(&off, colorWriteOffBlend_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateBlendState color mask failed.");
            return false;
        }

        D3D11_DEPTH_STENCIL_DESC write{};
        write.DepthEnable = TRUE;
        write.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        write.DepthFunc = D3D11_COMPARISON_LESS;
        write.StencilEnable = TRUE;
        write.StencilReadMask = 0xff;
        write.StencilWriteMask = 0xff;
        write.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        write.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
        write.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        write.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        write.BackFace = write.FrontFace;
        hr = device_->CreateDepthStencilState(&write, stencilWriteState_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateDepthStencilState write failed.");
            return false;
        }

        D3D11_DEPTH_STENCIL_DESC test{};
        test.DepthEnable = TRUE;
        test.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        test.DepthFunc = D3D11_COMPARISON_LESS;
        test.StencilEnable = TRUE;
        test.StencilReadMask = 0xff;
        test.StencilWriteMask = 0;
        test.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
        test.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        test.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        test.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        test.BackFace = test.FrontFace;
        hr = device_->CreateDepthStencilState(&test, stencilTestState_.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateDepthStencilState test failed.");
            return false;
        }
        return true;
    }

    bool CreateMeshes() {
        auto triangle = MakeTriangle();
        auto cube = kind_ == SampleKind::TexturedCube ? MakeTexturedCube(variant_ == 26 ? 3.0f : 1.0f) : MakeCube();
        auto plane = MakePlane(-1.0f, 4.0f, {0.8f, 0.8f, 0.82f, 1});
        auto wall = MakeWall(2.8f, 3.2f, {0.45f, 0.55f, 0.72f, 1});
        auto object = MakeObjectMesh();
        auto chair = MakeChair(6);
        objectIndexCount_ = static_cast<UINT>(object.indices.size());
        chairIndexCount_ = static_cast<UINT>(chair.indices.size());
        return CreateMesh(triangle, triangleVB_, triangleIB_) &&
            CreateMesh(cube, cubeVB_, cubeIB_) &&
            CreateMesh(plane, planeVB_, planeIB_) &&
            CreateMesh(wall, wallVB_, wallIB_) &&
            CreateMesh(object, objectVB_, objectIB_) &&
            CreateMesh(chair, chairVB_, chairIB_);
    }

    MeshData MakeObjectMesh() const {
        switch (kind_) {
        case SampleKind::Arrowhead:
            return MakeArrowhead(variant_);
        case SampleKind::Chair:
            return MakeChair(variant_);
        case SampleKind::Octahedron:
            return MakeOctahedron(variant_);
        case SampleKind::Tetrahedron:
            return MakeTetrahedron(variant_);
        case SampleKind::TexturedCube:
            return MakeTexturedCube(variant_ == 27 ? 2.0f : 1.0f);
        case SampleKind::Cube:
            return variant_ == 18 || variant_ == 19 ? MakeOctahedron(20) : MakeCube();
        default:
            return MakeCube();
        }
    }

    bool CreateMesh(const MeshData& mesh, ComPtr<ID3D11Buffer>& vb, ComPtr<ID3D11Buffer>& ib) {
        D3D11_BUFFER_DESC vbd{};
        vbd.ByteWidth = static_cast<UINT>(mesh.vertices.size() * sizeof(Vertex));
        vbd.Usage = D3D11_USAGE_DEFAULT;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA vinit{mesh.vertices.data(), 0, 0};
        HRESULT hr = device_->CreateBuffer(&vbd, &vinit, vb.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateBuffer vertex buffer failed.");
            return false;
        }

        D3D11_BUFFER_DESC ibd{};
        ibd.ByteWidth = static_cast<UINT>(mesh.indices.size() * sizeof(uint16_t));
        ibd.Usage = D3D11_USAGE_DEFAULT;
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        D3D11_SUBRESOURCE_DATA iinit{mesh.indices.data(), 0, 0};
        hr = device_->CreateBuffer(&ibd, &iinit, ib.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateBuffer index buffer failed.");
            return false;
        }
        return true;
    }

    void DrawMesh(ID3D11Buffer* vb, ID3D11Buffer* ib, UINT indexCount, XMMATRIX world, bool textured, XMFLOAT4 tint) {
        XMVECTOR eye = (kind_ == SampleKind::TexturedCube || kind_ == SampleKind::MirrorShadow)
            ? XMVectorSet(cosf(cameraAngle_) * 6.0f, cameraHeight_, sinf(cameraAngle_) * 6.0f, 1.0f)
            : XMVectorSet(0.0f, 2.3f, -6.0f, 1.0f);
        XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
        XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        XMMATRIX view = XMMatrixLookAtLH(eye, at, up);

        Constants constants{};
        constants.worldViewProjection = XMMatrixTranspose(world * view * projection_);
        constants.world = XMMatrixTranspose(world);
        constants.lightDirection = {-0.4f, -0.8f, 0.25f, 0.0f};
        constants.tint = tint;
        constants.options = {textured ? 1.0f : 0.0f, 0, 0, 0};
        context_->UpdateSubresource(constantBuffer_.Get(), 0, nullptr, &constants, 0, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        context_->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
        context_->IASetIndexBuffer(ib, DXGI_FORMAT_R16_UINT, 0);
        context_->PSSetShaderResources(0, 1, texture_.GetAddressOf());
        context_->DrawIndexed(indexCount, 0, 0);
    }

    std::wstring TextureName() const {
        static const wchar_t* names[] = {
            L"assets\\crate.bmp",
            L"assets\\grid_red.bmp",
            L"assets\\grid_blue.bmp",
            L"assets\\grid_gold.bmp",
        };
        int base = 0;
        if (variant_ == 25) {
            base = 0;
        } else if (variant_ == 26) {
            base = 1;
        } else if (variant_ == 27) {
            base = 2;
        }
        int index = (base + textureOffset_) % ARRAYSIZE(names);
        return names[index];
    }

    bool LoadBmpTexture(const std::wstring& relativePath) {
        std::wstring path = FindAsset(relativePath);
        if (path.empty()) {
            Fail(L"Missing texture file: " + relativePath);
            return false;
        }

        std::ifstream file(path, std::ios::binary);
        if (!file) {
            Fail(L"Cannot open texture file: " + path);
            return false;
        }

        std::array<uint8_t, 54> header{};
        file.read(reinterpret_cast<char*>(header.data()), header.size());
        if (file.gcount() != static_cast<std::streamsize>(header.size()) || header[0] != 'B' || header[1] != 'M') {
            Fail(L"Invalid BMP texture: " + path);
            return false;
        }

        uint32_t dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);
        int32_t width = *reinterpret_cast<int32_t*>(&header[18]);
        int32_t height = *reinterpret_cast<int32_t*>(&header[22]);
        uint16_t bitsPerPixel = *reinterpret_cast<uint16_t*>(&header[28]);
        uint32_t compression = *reinterpret_cast<uint32_t*>(&header[30]);

        if (width <= 0 || height <= 0 || bitsPerPixel != 32 || compression != 0) {
            Fail(L"Only uncompressed 32-bit BMP textures are supported: " + path);
            return false;
        }

        std::vector<uint8_t> pixels(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
        file.seekg(dataOffset, std::ios::beg);
        file.read(reinterpret_cast<char*>(pixels.data()), pixels.size());
        if (!file) {
            Fail(L"Could not read BMP pixel data: " + path);
            return false;
        }

        for (size_t i = 0; i < pixels.size(); i += 4) {
            std::swap(pixels[i], pixels[i + 2]);
        }

        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = static_cast<UINT>(width);
        desc.Height = static_cast<UINT>(height);
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA init{};
        init.pSysMem = pixels.data();
        init.SysMemPitch = static_cast<UINT>(width * 4);

        ComPtr<ID3D11Texture2D> texture;
        HRESULT hr = device_->CreateTexture2D(&desc, &init, texture.GetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateTexture2D texture failed.");
            return false;
        }

        hr = device_->CreateShaderResourceView(texture.Get(), nullptr, texture_.ReleaseAndGetAddressOf());
        if (FAILED(hr)) {
            Fail(L"CreateShaderResourceView failed.");
            return false;
        }
        return true;
    }

    SampleKind kind_;
    int variant_;
    int textureOffset_ = 0;
    float rotation_ = 0.0f;
    float cameraAngle_ = XM_PI * 1.5f;
    float cameraHeight_ = 2.4f;
    XMMATRIX projection_ = XMMatrixIdentity();

    ComPtr<ID3D11VertexShader> vertexShader_;
    ComPtr<ID3D11PixelShader> pixelShader_;
    ComPtr<ID3D11InputLayout> inputLayout_;
    ComPtr<ID3D11Buffer> constantBuffer_;
    ComPtr<ID3D11SamplerState> sampler_;
    ComPtr<ID3D11ShaderResourceView> texture_;
    ComPtr<ID3D11BlendState> alphaBlendState_;
    ComPtr<ID3D11BlendState> colorWriteOffBlend_;
    ComPtr<ID3D11DepthStencilState> stencilWriteState_;
    ComPtr<ID3D11DepthStencilState> stencilTestState_;

    ComPtr<ID3D11Buffer> triangleVB_;
    ComPtr<ID3D11Buffer> triangleIB_;
    ComPtr<ID3D11Buffer> cubeVB_;
    ComPtr<ID3D11Buffer> cubeIB_;
    ComPtr<ID3D11Buffer> planeVB_;
    ComPtr<ID3D11Buffer> planeIB_;
    ComPtr<ID3D11Buffer> wallVB_;
    ComPtr<ID3D11Buffer> wallIB_;
    ComPtr<ID3D11Buffer> objectVB_;
    ComPtr<ID3D11Buffer> objectIB_;
    ComPtr<ID3D11Buffer> chairVB_;
    ComPtr<ID3D11Buffer> chairIB_;
    UINT objectIndexCount_ = 0;
    UINT chairIndexCount_ = 0;
};
}

int RunTeachingSample(HINSTANCE instance, const wchar_t* title, SampleKind kind, int variant) {
    TeachingSampleApp app(instance, title, kind, variant);
    return app.Run();
}
