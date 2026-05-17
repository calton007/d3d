#include "../../common/SampleScene.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int)
{
    return RunTeachingSample(instance, L"tetrahedron_1", SampleKind::Tetrahedron, 33);
}
