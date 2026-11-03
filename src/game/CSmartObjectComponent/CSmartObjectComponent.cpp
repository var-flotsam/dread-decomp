// CSmartObjectComponent's static initialiser: the unit header, the class's registration, the
// component's id and two event names.
#include "base/UnkStrIdRegistry.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

// The unit's data (its statics, merged into one block).
struct UnkSmartObjectTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId id;          // 0x20
    CStrId use_init;    // 0x28
    CStrId use_abort;   // 0x30
    u8 field_0x38[0x20];
    UnkReflType type;   // 0x58
};

extern UnkSmartObjectTU g_unk_7172742d80 __attribute__((visibility("hidden")));
extern char g_unk_7172742c68;

void fn_7100a05880() {
    UnkSmartObjectTU& tu = g_unk_7172742d80;
    InitHeader(tu.header);
    fn_7100a00014();
    fn_710009ec00(&g_unk_7172742c68, &tu.type);
    new (&tu.id) CStrId(fn_7100de8f78(CStrId("SMARTOBJS", true)));
    fn_7100000250(fn_710000051c, &tu.id, &g_unk_7101a19000);
    new (&tu.use_init) CStrId("useinit", true);
    fn_7100000250(fn_710000051c, &tu.use_init, &g_unk_7101a19000);
    new (&tu.use_abort) CStrId("useabort", true);
    fn_7100000250(fn_710000051c, &tu.use_abort, &g_unk_7101a19000);
}
