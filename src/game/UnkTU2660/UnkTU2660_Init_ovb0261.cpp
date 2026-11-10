// Unit 2660 (tools/tu_map.py, no reliable name): the static initialiser that registers the class
// CScenarioLogicImporter (helpers in game/UnkTUInit.h). Names and layouts are placeholders.
#include "game/UnkTUInit.h"

struct UnkTUData_b0261 {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
    u8 unk_0x18[0x10];
    UnkFnRecord fns[3];
};

extern UnkTUData_b0261 g_unk_7172a000a8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729fff68;
extern char g_unk_7172a000a0;
void fn_71010621bc();
void fn_7101062210(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);

void fn_7101062570() {
    InitHeaderFns(g_unk_7172a000a8);
    InitClass(&g_unk_71729fff68, "CScenarioLogicImporter", fn_7101062210, fn_71010621bc);
    fn_710009ec00(&g_unk_7172a000a0, &g_unk_71729fff68);
}
