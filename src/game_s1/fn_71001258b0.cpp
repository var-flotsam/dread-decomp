// The static initialiser of base::spatial::CPolygonCollection2D's unit (helpers in game/UnkTUInit.h), whose
// header is built in the other order.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

// The header built the other way round: "sources" first, the word last.
struct UnkTUHeaderRev {
    u32 field_0x0;
    CStrId sources;
    CStrId binaries;
};

__attribute__((always_inline)) static inline void InitHeaderRev(UnkTUHeaderRev& h) {
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &h.sources, &g_unk_7101a19000);
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &h.binaries, &g_unk_7101a19000);
    h.field_0x0 = 6;
}

extern UnkTUHeaderRev g_unk_7108b6feb8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7108b6fd78;
extern char g_unk_7108b6feb0;
void fn_7100123d30(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001239b0();

void fn_71001258b0() {
    InitHeaderRev(g_unk_7108b6feb8);
    InitClass(&g_unk_7108b6fd78, "base::spatial::CPolygonCollection2D", fn_7100123d30, fn_71001239b0);
    fn_710009ec00(&g_unk_7108b6feb0, &g_unk_7108b6fd78);
}
