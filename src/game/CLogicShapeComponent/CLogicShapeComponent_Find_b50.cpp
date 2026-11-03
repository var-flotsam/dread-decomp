// CLogicShapeComponent unit (tools/tu_map.py): the CLogicShapeComponent that the game manager's list for
// the class id (fn_7100de94cc) holds for `key` (fn_710024e2e8), when it is of the class (fn_710009d50c
// against the type in the unit's TU data; the getter runs first so the type is registered), else null.
// Same shape as src/game/CLandmarkComponent/CLandmarkComponent_Find_b49a4.cpp. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;

extern CStrId g_unk_7172736760 __attribute__((visibility("hidden")));  // CLogicShapeComponent's id
extern CType g_unk_7172736768 __attribute__((visibility("hidden")));   // its type

UnkReflObject* fn_710024e2e8(UnkGameManagerIdList* list, void* key);

UnkReflObject* fn_71009143d8(void* key) {
    UnkGameManagerIdList* list =
        fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172736760));
    UnkReflObject* obj = fn_710024e2e8(list, key);
    if (obj == nullptr)
        return nullptr;
    const CType* type = obj->vf_0x0();
    fn_7100912a38();
    return fn_710009d50c(type, &g_unk_7172736768) ? obj : nullptr;
}
