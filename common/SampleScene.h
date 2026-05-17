#pragma once

#include "D3DApp.h"

enum class SampleKind {
    Triangle,
    Cube,
    TexturedCube,
    MirrorShadow,
    Arrowhead,
    Chair,
    Octahedron,
    Tetrahedron,
    Utility,
};

int RunTeachingSample(HINSTANCE instance, const wchar_t* title, SampleKind kind, int variant);
