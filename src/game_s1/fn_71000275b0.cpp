// The static initialiser of base::core::CAssetLink's unit (helpers in game/UnkTUInit.h): the header ids in the
// other order, the class, and its converters from string ids and strings and to strings.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTU32.h"
#include "game/UnkTUInit.h"

// The header ids built the other way round ("sources" first, no word stored).
struct UnkTUIdsRev {
    CStrId sources;
    CStrId binaries;
};

__attribute__((always_inline)) static inline void InitIdsRev(UnkTUIdsRev& h) {
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(CStrId_Destroy, &h.sources, &g_unk_7101a19000);
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(CStrId_Destroy, &h.binaries, &g_unk_7101a19000);
}

extern UnkTUIdsRev g_unk_7101d4ee10 __attribute__((visibility("hidden")));
extern char g_unk_7101d4ed88;
extern UnkConverter g_unk_7101d4ed90;
extern UnkConverter g_unk_7101d4edb0;
extern UnkConverter g_unk_7101d4edd0;
void fn_7100026500(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71000262b8();
void* AssignAssetLinkFromPathText(void*, void*);
void* CopyAssignAssetLink(void*, void*);
void fn_7100026cf8() __attribute__((visibility("hidden")));

void InitCAssetLinkStatics() {
    InitIdsRev(g_unk_7101d4ee10);
    InitClass(&reinterpret_cast<UnkReflType&>(g_unk_7101d4ec50), "base::core::CAssetLink", fn_7100026500,
              fn_71000262b8);
    fn_710009ec00(&g_unk_7101d4ed88, &reinterpret_cast<UnkReflType&>(g_unk_7101d4ec50));
    InitConverterFrom(g_unk_7101d4ed90, &reinterpret_cast<UnkReflType&>(g_unk_7101d4ec50), refl_TypeOf<fn_7100007f84>,
                      reinterpret_cast<void (*)()>(&AssignAssetLinkFromPathText));
    InitConverterFrom(g_unk_7101d4edb0, &reinterpret_cast<UnkReflType&>(g_unk_7101d4ec50), refl_TypeOf<fn_7100009658>,
                      reinterpret_cast<void (*)()>(&CopyAssignAssetLink));
    InitConverterTo(g_unk_7101d4edd0, &reinterpret_cast<UnkReflType&>(g_unk_7101d4ec50), refl_TypeOf<fn_7100007f84>,
                    fn_7100026cf8);
}
