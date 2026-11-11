// CBreakableTileGroupSonarTargetComponent unit (tools/tu_map.py unit 1517): runs the base step
// (fn_71010c4228), then, when fn_71002a5a58 finds an object for the owner at +0x30, points the
// counted weak reference at +0x40 at that object's weak handle (created on first use;
// game_s2/UnkWeakHandle.h). Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Signature as the other hand-written callers declare it.
struct UnkParent;
UnkWeakTarget* fn_71002a5a58(void* owner);

struct UnkBreakableTileGroupSonarTarget_b52 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkWeakHandle* handle;  // 0x40
};

void fn_7100a0e074(UnkBreakableTileGroupSonarTarget_b52* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    if (UnkWeakTarget* target = fn_71002a5a58(self->owner))
        AssignRef(&self->handle, GetWeakHandle(target));
}
