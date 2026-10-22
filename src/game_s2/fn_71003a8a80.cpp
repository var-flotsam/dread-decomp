// Static initialiser of the unit of the behaviortree::CParallel::EPolicy.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71003a8a80 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71003a8a80 g_unk_71726bbd18 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726bbbb0;
void fn_71003a76bc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003a7570();
extern char g_unk_71726bbcc8[];
extern UnkConversion g_unk_71726bbcd0;
extern UnkConversion g_unk_71726bbcf0;
void fn_71003a879c() __attribute__((visibility("hidden")));
void fn_71003a87a8() __attribute__((visibility("hidden")));
void* fn_71003a7840();  // behaviortree::CParallel::refl_GetClass
extern char g_unk_71726bbd10[];

void fn_71003a8a80() {
    InitHeaderIds(&g_unk_71726bbd18.ids);
    InitEnum(&g_unk_71726bbbb0, "behaviortree::CParallel::EPolicy", fn_71003a76bc, fn_71003a7570,
             g_unk_71726bbcc8, &g_unk_71726bbcd0, fn_71003a879c, &g_unk_71726bbcf0, fn_71003a87a8);
    fn_71003a7840();
    fn_710009ec00(g_unk_71726bbd10, g_unk_71726bbd18.field_0x20);
}
