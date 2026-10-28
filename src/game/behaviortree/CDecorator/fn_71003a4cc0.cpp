// Makes the weak reference at +0x20 of `holder` refer to the weak handle of `target` (created on
// first use), or to nothing when `target` is null or has no handle slot, then points the target's
// +0x30 back at the holder. Does nothing without a holder.
#include "game_s2/UnkWeakHandle.h"

struct UnkHolder {
    u8 pad_0x0[0x20];
    UnkWeakHandle* field_0x20;
};

struct UnkOwnedTarget : UnkWeakTarget {
    u8 pad_0x8[0x28];
    UnkHolder* field_0x30;
};

void fn_71003a4cc0(UnkOwnedTarget* target, UnkHolder* holder) {
    if (!holder)
        return;
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(&holder->field_0x20, handle);
    target->field_0x30 = holder;
}
