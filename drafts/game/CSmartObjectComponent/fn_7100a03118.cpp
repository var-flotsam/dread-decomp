// CSmartObjectComponent unit (tools/tu_map.py unit 1512): links the component to the object its id
// names (once, while the link at +0xb0 leads nowhere), to that object's user, and the user back to
// the component's owner. Returns whether the link leads somewhere. Names and layouts are
// placeholders. Returns bool: include/game/CSmartObjectComponent.h declares it void (to change).
#include "base/global/CStrId.h"
#include "game/CElectricReactionComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// The object a smart object's user is: its link back at +0xe0.
struct UnkSmartObjectUser_a03118 : UnkWeakTarget {
    u8 pad_0x8[0xd8];
    UnkWeakHandle* field_0xe0;
};

struct UnkSmartObjectComponent_a03118 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x28];
    CStrId target_id;      // 0x60
    u8 pad_0x68[0x48];
    UnkWeakHandle* target;  // 0xb0
    UnkWeakHandle* user;    // 0xb8
};
static_assert(__builtin_offsetof(UnkSmartObjectComponent_a03118, user) == 0xb8, "layout");

// A counted reference held for a scope.
struct UnkHeldRef_a03118 {
    UnkWeakHandle* ptr;

    explicit UnkHeldRef_a03118(UnkWeakHandle* p) : ptr(p) {
        if (ptr)
            ++ptr->ref_count;
    }
    ~UnkHeldRef_a03118() {
        if (ptr) {
            if (--ptr->ref_count == 0)
                ptr->vf_0x10();
        }
    }
};

// The object of `table` whose key is `key`.
inline UnkWeakTarget* FindObject(UnkGameManagerSub_0x608* table, u64 key) {
    return static_cast<UnkWeakTarget*>(fn_71011a3d74(table, &key));
}

bool fn_7100a03118(void* p) {
    auto* self = static_cast<UnkSmartObjectComponent_a03118*>(p);
    if (!(self->target && self->target->target) && self->target_id.data != &g_unk_7101d47108->empty) {
        if (UnkWeakTarget* found = FindObject(g_unk_7101d47310->field_0x608, self->target_id.data->Key())) {
            UnkWeakHandle* handle = GetWeakHandle(found);
            AssignRef(&self->target, handle);
            if (handle && handle->target) {
                auto* user = reinterpret_cast<UnkWeakTarget*>(
                    fn_71007cfd8c(reinterpret_cast<UnkSmartObjectActor*>(handle->target)));
                if (user) {
                    UnkWeakHandle* user_handle = GetWeakHandle(user);
                    AssignRef(&self->user, user_handle);
                    auto* linked =
                        static_cast<UnkSmartObjectUser_a03118*>(user_handle ? user_handle->target : nullptr);
                    UnkHeldRef_a03118 owner(fn_7100d78378(self->owner));
                    CopyWeakRef(&linked->field_0xe0, owner.ptr);
                }
            }
        }
    }
    return self->target && self->target->target;
}
