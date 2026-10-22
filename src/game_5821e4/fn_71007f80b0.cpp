// Static initialiser of CFrozenAsFrostbiteComponent's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71007f80b0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkFilePathStrIdBase field_0x20;
    UnkFilePathStrIdBase field_0x28;
    u8 pad_0x30[0x10];
    u8 field_0x40[0x10];
    u8 field_0x50[8];
    u8 pad_0x58[0x130];
    u8 field_0x188[8];
};
extern UnkStatics_71007f80b0 g_unk_7172720e00 __attribute__((visibility("hidden")));
void fn_710058a2b8(void* self);
void* fn_71007f3534();  // CFrozenAsFrostbiteComponent::refl_GetClass
void* fn_71007f3790();
extern char g_unk_7172720db8[];
extern char g_unk_7172720db9[];
extern UnkTunable g_unk_7172720dc0;
void fn_7100136ecc(UnkTunable* self);
extern void* g_unk_7101a740a0[];
void fn_7100075104(void* self);

void fn_71007f80b0() {
    InitHeaderIds(&g_unk_7172720e00.ids);
    InitId(&g_unk_7172720e00.field_0x18, "aSS");
    InitPathId(&g_unk_7172720e00.field_0x20, "actors/props/ice_cube/fx/icemissile_frozenfx.bcptl");
    InitPathId(&g_unk_7172720e00.field_0x28, "actors/fx/ice_explosion_broken/ice_explosion_broken.bcptl");
    {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("icesteam", true), UnkStrIdTmp("icesteam1", true)};
        fn_710058a070(g_unk_7172720e00.field_0x40, ids, 2);
    }
    __cxa_atexit(fn_710058a2b8, g_unk_7172720e00.field_0x40, g_unk_7101a19000);
    fn_71007f3534();
    fn_710009ec00(g_unk_7172720db8, g_unk_7172720e00.field_0x50);
    fn_71007f3790();
    fn_710009ec00(g_unk_7172720db9, g_unk_7172720e00.field_0x188);
    InitTunable(&g_unk_7172720dc0, fn_7100136ecc, g_unk_7101a740a0, fn_7100075104);
}
