// navmesh::CNavMesh unit (tools/tu_map.py, the name from reflection): passes `arg` to one of two
// deferred callback sets (+0x190 when `first`, else +0x210) through the generated clone
// fn_7100280c9c (base::refl_DeferredInvokePtr). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "types.h"

struct UnkClone_fn_7100280c9c;
extern template void base::refl_DeferredInvokePtr<UnkClone_fn_7100280c9c>(
    base::UnkDeferredSet<base::UnkDeferredCallback<const void*>>*, const void*);

typedef base::UnkDeferredSet<base::UnkDeferredCallback<const void*>> UnkNavMeshCallbacks_b65a1;

struct UnkNavMesh_b65a1 {
    u8 pad_0x0[0x190];
    u8 first[0x80];
    u8 second[0x80];
};

void fn_710027daa4(UnkNavMesh_b65a1* self, const void* arg, bool first) {
    u8* set = !first ? self->second : self->first;
    base::refl_DeferredInvokePtr<UnkClone_fn_7100280c9c>(reinterpret_cast<UnkNavMeshCallbacks_b65a1*>(set), arg);
}
