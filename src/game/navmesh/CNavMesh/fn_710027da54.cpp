// navmesh::CNavMesh: sends a 16-bit value to the deferred callback set at +0x90 or +0x110 by flag.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "game/navmesh/CNavMesh.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;

struct UnkNavMesh_l27da54 {
    u8 pad_0x0[0x90];
    UnkDeferredSet<UnkDeferredCallback<u16>> field_0x90;
    u8 pad_0x90_end[0x110 - 0x90 - sizeof(UnkDeferredSet<UnkDeferredCallback<u16>>)];
    UnkDeferredSet<UnkDeferredCallback<u16>> field_0x110;
};

void fn_7100280934(UnkDeferredSet<UnkDeferredCallback<u16>>* s, const u16* arg);

void CNavMesh_NotifyCellFlagChanged(UnkNavMesh_l27da54* self, const u16* value, bool first) {
    if (first) {
        u16 arg = *value;
        fn_7100280934(&self->field_0x90, &arg);
    } else {
        u16 arg = *value;
        fn_7100280934(&self->field_0x110, &arg);
    }
}
