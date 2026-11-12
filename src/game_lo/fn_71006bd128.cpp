// fn_71006bd128: forgets the held handle at +0x8 when it refers to `target`, then hands a counted
// reference to the target's weak handle (fn_7100d78378) to the list at +0x10 (fn_710059cecc).
// Nothing happens for a null target. The unit has no class hint; names are placeholders.
#include "game/UnkTU2523.h"
#include "game/UnkTU852.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The list at +0x10; fn_710059cecc adds a reference to it.
struct UnkHandleList {
    void* field_0x0;
};
static inline UnkRefPtr MakeRef(UnkWeakHandle* handle) {
    UnkRefPtr ref;
    ref.field_0x0 = nullptr;
    if (handle) {
        ++handle->ref_count;
        ref.field_0x0 = handle;
    }
    return ref;
}

struct UnkTargetHolder {
    u8 pad_0x0[0x8];
    UnkWeakHandle* field_0x8;
    UnkHandleList field_0x10;
};

void fn_71006bd128(UnkTargetHolder* self, UnkWeakTarget* target) {
    if (!target)
        return;
    if (UnkWeakHandle* held = self->field_0x8) {
        if (held->target && held->target == target)
            ResetRef(&self->field_0x8);
    }
    UnkHandleList* list = &self->field_0x10;
    fn_710059cecc(list, MakeRef(fn_7100d78378(target)));
}
