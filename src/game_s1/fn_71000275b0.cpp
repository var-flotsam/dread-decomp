// The static initialiser of base::core::CAssetLink's unit (helpers in game/UnkTUInit.h): the header ids in the
// other order, the class, and its converters from string ids and strings and to strings.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

// The header ids built the other way round ("sources" first, no word stored).
struct UnkTUIdsRev {
    CStrId sources;
    CStrId binaries;
};

__attribute__((always_inline)) static inline void InitIdsRev(UnkTUIdsRev& h) {
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &h.sources, &g_unk_7101a19000);
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &h.binaries, &g_unk_7101a19000);
}

extern UnkTUIdsRev g_unk_7101d4ee10 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7101d4ec50;
extern char g_unk_7101d4ed88;
extern UnkConverter g_unk_7101d4ed90;
extern UnkConverter g_unk_7101d4edb0;
extern UnkConverter g_unk_7101d4edd0;
void fn_7100026500(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71000262b8();
void fn_71000272cc();
void fn_7100027418();
void fn_7100026cf8() __attribute__((visibility("hidden")));
UnkReflType* fn_7100007f84();  // base::reflection::refl_GetType<base::global::CRntString>
UnkReflType* fn_7100009658();  // base::reflection::refl_GetType<base::global::CStrId>

void fn_71000275b0() {
    InitIdsRev(g_unk_7101d4ee10);
    InitClass(&g_unk_7101d4ec50, "base::core::CAssetLink", fn_7100026500, fn_71000262b8);
    fn_710009ec00(&g_unk_7101d4ed88, &g_unk_7101d4ec50);
    InitConverterFrom(g_unk_7101d4ed90, &g_unk_7101d4ec50, fn_7100007f84, fn_71000272cc);
    InitConverterFrom(g_unk_7101d4edb0, &g_unk_7101d4ec50, fn_7100009658, fn_7100027418);
    InitConverterTo(g_unk_7101d4edd0, &g_unk_7101d4ec50, fn_7100007f84, fn_7100026cf8);
}
