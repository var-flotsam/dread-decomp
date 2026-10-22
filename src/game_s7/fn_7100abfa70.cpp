// Copies of fn_71007a10c0 (src/game_5821e4/fn_71007a10c0.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7100abfa70, fn_7100ac87b0.
// Static initialiser of CCubeMapComponent's unit.
#include "../game_5821e4/UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_0x30 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_0x30 g_unk_7172754e48 __attribute__((visibility("hidden")));
extern UnkStatics_0x30 g_unk_7172755840 __attribute__((visibility("hidden")));
void* fn_710079d3b0();  // CCubeMapComponent::refl_GetClass
extern char g_unk_7172718d54[];

void* fn_7100ab9ae0();
void* fn_7100ac6c48();
void fn_7100abfa70() {
    InitHeaderIds(&g_unk_7172754e48.ids);
    fn_7100ab9ae0();
    fn_710009ec00(g_unk_7172718d54, g_unk_7172754e48.field_0x28);
    InitClassId(&g_unk_7172754e48.field_0x20, "CCubeMapComponent");
}

void fn_7100ac87b0() {
    InitHeaderIds(&g_unk_7172755840.ids);
    fn_7100ac6c48();
    fn_710009ec00(g_unk_7172718d54, g_unk_7172755840.field_0x28);
    InitClassId(&g_unk_7172755840.field_0x20, "CCubeMapComponent");
}

