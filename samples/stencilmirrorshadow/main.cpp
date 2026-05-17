#include "../../common/SampleScene.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int)
{
    return RunTeachingSample(instance, L"stencilmirrorshadow", SampleKind::MirrorShadow, 32);
}
