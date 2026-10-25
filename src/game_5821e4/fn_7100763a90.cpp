// Static initialiser of CBreakableVignetteComponent's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"

using namespace unk_static_init;

struct UnkStatics_7100763a90 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_7100763a90 g_unk_7172713408 __attribute__((visibility("hidden")));
extern UnkProperty2 g_unk_71727133c0;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];
extern UnkEnumType g_unk_7172735f58;
void fn_71004fa3b8(void* p);
extern char g_unk_7172713400[];

void fn_7100763a90() {
    InitHeaderIds(&g_unk_7172713408.ids);
    InitProperty2(&g_unk_71727133c0, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, "OCCLUDER_VIGNETTES", &g_unk_7172735f58,
                  fn_71004fd2cc, fn_71004fa3b8);
    fn_710076238c();
    fn_710009ec00(g_unk_7172713400, g_unk_7172713408.field_0x28);
    InitClassId(&g_unk_7172713408.field_0x20, "CBreakableVignetteComponent");
}
