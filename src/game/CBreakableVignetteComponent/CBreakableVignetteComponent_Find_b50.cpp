// CBreakableVignetteComponent unit (tools/tu_map.py): fn_7100762b8c, the shape of
// CLandmarkComponent_Find_b49a4.cpp: the component of this class that the game manager's list for
// the class id (fn_7100de94cc) holds for `key` (fn_710024e2e8), when it is of the class
// (fn_710009d50c against the type in the unit's TU data; the getter runs first so the type is
// registered), else null. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;

extern CStrId g_unk_7172713428 __attribute__((visibility("hidden")));  // CBreakableVignetteComponent's id
extern CType g_unk_7172713430 __attribute__((visibility("hidden")));   // its type

UnkReflObject* fn_710024e2e8(UnkGameManagerIdList* list, void* key);

UnkReflObject* fn_7100762b8c(void* key) {
    UnkGameManagerIdList* list = fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172713428));
    UnkReflObject* obj = fn_710024e2e8(list, key);
    if (obj == nullptr)
        return nullptr;
    const CType* type = obj->vf_0x0();
    fn_710076238c();
    return fn_710009d50c(type, &g_unk_7172713430) ? obj : nullptr;
}
