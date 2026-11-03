// CLogicPathComponent unit (tools/tu_map.py): the CLogicPathComponent that the game manager's list for
// the class id (fn_7100de94cc) holds for the id `key` (fn_710024e2e8), when it is of the class
// (fn_710009d50c against the type in the unit's TU data; the getter runs first so the type is
// registered), else null. Same shape as src/game/CLandmarkComponent/CLandmarkComponent_Find_b49a4.cpp;
// the signature is the one src/game/CAIComponent/CAIComponent_b41.cpp calls. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;

extern CStrId g_unk_7172735b90 __attribute__((visibility("hidden")));  // CLogicPathComponent's id
extern CType g_unk_7172735b98 __attribute__((visibility("hidden")));   // its type

UnkReflObject* fn_710024e2e8(UnkGameManagerIdList* list, void* key);

void* fn_7100912674(const CStrId* key) {
    UnkGameManagerIdList* list =
        fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172735b90));
    UnkReflObject* obj = fn_710024e2e8(list, const_cast<CStrId*>(key));
    if (obj == nullptr)
        return nullptr;
    const CType* type = obj->vf_0x0();
    fn_71009120e4();
    return fn_710009d50c(type, &g_unk_7172735b98) ? obj : nullptr;
}
