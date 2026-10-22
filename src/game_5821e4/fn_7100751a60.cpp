// Static initialiser of CBreakableScenarioComponent's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100751a60 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkStrIdBase field_0x28;
    u8 field_0x30[8];
    u8 pad_0x38[0x130];
    u8 field_0x168[8];
};
extern UnkStatics_7100751a60 g_unk_71727110f0 __attribute__((visibility("hidden")));
extern UnkProperty2 g_unk_7172711098;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];
extern UnkEnumType g_unk_717273e7f0;
void fn_71004fd2cc(UnkProperty2* p);
void fn_71004fa3b8(void* p);
void* fn_7100750d48();  // CCharClassBreakableScenarioComponent::refl_GetClass
extern char g_unk_71727110d8[];
void* fn_7100751108();  // CBreakableScenarioComponent::refl_GetClass
extern char g_unk_71727110d9[];

void fn_7100751a60() {
    InitHeaderIds(&g_unk_71727110f0.ids);
    InitProperty2(&g_unk_7172711098, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, "BREAKABLE_OCCLUDERS", &g_unk_717273e7f0,
                  fn_71004fd2cc, fn_71004fa3b8);
    fn_7100750d48();
    fn_710009ec00(g_unk_71727110d8, g_unk_71727110f0.field_0x30);
    fn_7100751108();
    fn_710009ec00(g_unk_71727110d9, g_unk_71727110f0.field_0x168);
    InitClassId(&g_unk_71727110f0.field_0x28, "CBreakableScenarioComponent");
}
