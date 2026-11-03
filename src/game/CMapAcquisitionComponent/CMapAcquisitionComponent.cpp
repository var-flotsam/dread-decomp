// CMapAcquisitionComponent's static initialiser: the unit header, the component's id, a plain and
// an extended bool property, and the class's registration.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkMapAcquisitionTU {
    UnkTUHeader header;
    u64 field_0x18;
    UnkReflType type;  // 0x20
};

extern UnkMapAcquisitionTU g_unk_7172750e58 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172750da0;
extern UnkProperty g_unk_7172750da8;
extern UnkPropertyEx<> g_unk_7172750dd0;
extern char g_unk_7101a92110[];
extern void* g_unk_7101a45fe8[];
extern char g_unk_7101a92168[];
void fn_7100a88028(UnkProperty* p);
void fn_7100a84880(void* p);
void fn_7100a848f8(void* p);
extern char g_unk_7172750e10;

void fn_7100a88650() {
    UnkMapAcquisitionTU& tu = g_unk_7172750e58;
    InitHeader(tu.header);
    new (&g_unk_7172750da0) CStrId("MAP", true);
    fn_7100000250(fn_710000051c, &g_unk_7172750da0, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_7172750da8, reinterpret_cast<void**>(g_unk_7101a92110), g_unk_71726db788,
                            "Used", refl_TypeOf<fn_710000d228>(), fn_7100a88028, fn_7100a84880);
    InitPropertyEx(&g_unk_7172750dd0, g_unk_7101a45fe8, g_unk_71726db788,
                   reinterpret_cast<void**>(g_unk_7101a92168), fn_71002ef5bc, "Acquired",
                   refl_TypeOf<fn_710000d228>(), reinterpret_cast<void (*)(UnkPropertyEx<>*)>(fn_7100a88028),
                   fn_7100a848f8);
    fn_7100a849c8();
    fn_710009ec00(&g_unk_7172750e10, &tu.type);
}
