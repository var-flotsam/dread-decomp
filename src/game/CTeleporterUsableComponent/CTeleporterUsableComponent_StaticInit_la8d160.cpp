// Static initialiser of CTeleporterUsableComponent's unit (with the ETeleporterColorSphere enum): the
// sphere models' names, a list of them and a table of them by colour. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

// A sphere colour with its model's name.
struct UnkSphereByColor_la8d160 {
    u32 color;
    UnkStrIdBase name;
    UnkSphereByColor_la8d160(u32 c, const char* n) : color(c), name(n, true) {}
};

struct UnkStatics_7100a8d160 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    CStrId spheres[8];  // 0x20
    u8 pad_0x60[0x8];
    f32 field_0x68[2];
    bool field_0x70;
    u8 pad_0x71[0x7];
    CStrId sphereList[8];                          // 0x78
    UnkSphereByColor_la8d160 spheresByColor[8];  // 0xb8
    u8 field_0x138[8];
};
static_assert(__builtin_offsetof(UnkStatics_7100a8d160, spheresByColor) == 0xb8, "layout");
extern UnkStatics_7100a8d160 g_unk_71727519a8 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172751840;
void fn_7100a8cad4(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100a8c748(void* e);
extern char g_unk_7172751958[];
extern UnkConversion g_unk_7172751960;
extern UnkConversion g_unk_7172751980;
void fn_7100a8f7ec() __attribute__((visibility("hidden")));
void fn_7100a8f7f8() __attribute__((visibility("hidden")));
void fn_7100a8cc60(void*) __attribute__((visibility("hidden")));
void fn_7100a8cee0(void*) __attribute__((visibility("hidden")));
extern char g_unk_71727519a0[];

void fn_7100a8d160() {
    UnkStatics_7100a8d160& s = g_unk_71727519a8;
    InitHeaderIds(&s.ids);
    s.field_0x68[0] = -1.0f;
    s.field_0x68[1] = -1.0f;
    s.field_0x70 = true;
    InitEnum(&g_unk_7172751840, "ETeleporterColorSphere", fn_7100a8cad4, fn_7100a8c748, g_unk_7172751958,
             &g_unk_7172751960, fn_7100a8f7ec, &g_unk_7172751980, fn_7100a8f7f8);
    InitId(&s.field_0x18, "CTeleporterUsableComponent");
    InitId(&s.spheres[0], "teleporter_sphere_blue");
    InitId(&s.spheres[1], "teleporter_sphere_bluedark");
    InitId(&s.spheres[2], "teleporter_sphere_green");
    InitId(&s.spheres[3], "teleporter_sphere_orange");
    InitId(&s.spheres[4], "teleporter_sphere_pink");
    InitId(&s.spheres[5], "teleporter_sphere_purple");
    InitId(&s.spheres[6], "teleporter_sphere_red");
    InitId(&s.spheres[7], "teleporter_sphere_yellow");
    for (u32 i = 0; i < 8; ++i)
        new (&s.sphereList[i]) CStrId(s.spheres[i]);
    fn_7100000250(fn_7100a8cc60, nullptr, g_unk_7101a19000);
    new (&s.spheresByColor[0]) UnkSphereByColor_la8d160(0, "teleporter_sphere_blue");
    new (&s.spheresByColor[1]) UnkSphereByColor_la8d160(1, "teleporter_sphere_bluedark");
    new (&s.spheresByColor[2]) UnkSphereByColor_la8d160(2, "teleporter_sphere_green");
    new (&s.spheresByColor[3]) UnkSphereByColor_la8d160(3, "teleporter_sphere_orange");
    new (&s.spheresByColor[4]) UnkSphereByColor_la8d160(4, "teleporter_sphere_pink");
    new (&s.spheresByColor[5]) UnkSphereByColor_la8d160(5, "teleporter_sphere_purple");
    new (&s.spheresByColor[6]) UnkSphereByColor_la8d160(6, "teleporter_sphere_red");
    new (&s.spheresByColor[7]) UnkSphereByColor_la8d160(7, "teleporter_sphere_yellow");
    fn_7100000250(fn_7100a8cee0, nullptr, g_unk_7101a19000);
    fn_7100a8d6d4();
    fn_710009ec00(g_unk_71727519a0, s.field_0x138);
}
