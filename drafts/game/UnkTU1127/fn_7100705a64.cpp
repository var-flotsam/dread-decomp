// Unit 1127 (tools/tu_map.py): goes through a list of weak handles and, for each one whose target is
// a door lock, tells the door (fn_71008d0740, removing the owner's id) and drops the handle from the
// list. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU367.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_71008d0740(UnkDoorLife_b44a2* self, bool add, base::global::CStrId* key);

// A counted reference to a weak handle.
struct UnkWeakRef_l705a64 {
    UnkWeakHandle* handle;

    UnkWeakRef_l705a64(const UnkWeakRef_l705a64& other) : handle(nullptr) { AssignRef(&handle, other.handle); }
    ~UnkWeakRef_l705a64() {
        if (UnkWeakHandle* h = handle) {
            if (--h->ref_count == 0)
                h->vf_0x10();
        }
    }
};

struct UnkDoorOwner_l705a64 {
    u8 pad_0x0[0x30];
    base::global::CStrId field_0x30;
};

struct UnkDoorList_l705a64 {
    UnkDoorOwner_l705a64* owner;
    base::global::CRntVector<UnkWeakRef_l705a64> handles;  // 0x8
};

void fn_7100705a64(UnkDoorList_l705a64* self) {
    for (UnkWeakRef_l705a64* it = self->handles.data; self->handles.data + self->handles.count != it;) {
        UnkWeakHandle* handle = it->handle;
        if (handle && handle->target) {
            if (UnkDoorLock* door = fn_71002af860(handle->target)) {
                fn_71008d0740(reinterpret_cast<UnkDoorLife_b44a2*>(door), false, &self->owner->field_0x30);
                // Erases the handle (CRntVector::EraseAt, written out), moving the later ones down.
                base::global::CRntVector<UnkWeakRef_l705a64>& handles = self->handles;
                u32 index = it - handles.data;
                if (index < handles.count) {
                    handles.data[index].~UnkWeakRef_l705a64();
                    for (u32 i = index; i < handles.count - 1; ++i) {
                        new (&handles.data[i]) UnkWeakRef_l705a64(handles.data[i + 1]);
                        handles.data[i + 1].~UnkWeakRef_l705a64();
                    }
                    handles.count = handles.count - 1;
                }
                it = handles.data + index;
                continue;
            }
        }
        ++it;
    }
}
