// Two copies of a lookup in the CActor unit (unit 341 of tools/tu_map.py): the reflected object a
// name maps to in the actor's map of parallel arrays at +0xa0 (base/UnkParallelFind.h), or null.
// The generated statics call them (src/reflection_statics/).
#include "base/UnkParallelFind.h"
#include "base/reflection/UnkReflStatics.h"

using base::reflection::UnkReflObject;

struct UnkActorObjectMap {
    u8 pad_0x0[0xa0];
    UnkReflObject** values;      // 0xa0
    base::global::CStrId* ids;   // 0xa8
    u32 count;                   // 0xb0
};

UnkReflObject* fn_710023341c(void* owner, const base::global::CStrId& name) {
    return UnkParallelFind(static_cast<UnkActorObjectMap*>(owner), name);
}

UnkReflObject* fn_710023348c(void* owner, const base::global::CStrId& name) {
    return UnkParallelFind(static_cast<UnkActorObjectMap*>(owner), name);
}
