#include "../../common/SampleScene.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int)
{
    return RunTeachingSample(instance, L"triangle", SampleKind::Triangle, 40);
}
