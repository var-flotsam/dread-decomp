// Static initialiser of a reflected component's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100753350 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_7100753350 g_unk_71727113d8 __attribute__((visibility("hidden")));
extern UnkProperty2 g_unk_7172711390;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];
extern UnkEnumType g_unk_7172735f58;
void fn_71004fd2cc(UnkProperty2* p);
void fn_71004fa3b8(void* p);
void* fn_7100751c60();  // CBreakableHintComponent::refl_GetClass
extern char g_unk_71727113d0[];

void fn_7100753350() {
    InitHeaderIds(&g_unk_71727113d8.ids);
    InitProperty2(&g_unk_7172711390, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, "BREAKABLE_HINTS", &g_unk_7172735f58,
                  fn_71004fd2cc, fn_71004fa3b8);
    fn_7100751c60();
    fn_710009ec00(g_unk_71727113d0, g_unk_71727113d8.field_0x20);
}
