// CTriggerComponent's unit: erasing the owned object at an index from an array of owned pointers,
// moving the later ones down. Names and layouts are placeholders.
#include <new>
#include "base/global/CRntVector.h"
#include "game/CTriggerComponent.h"
#include "types.h"

struct UnkTriggerOwned_b56;

struct UnkOwnedTrigger_la63ae4 {
    union {
        UnkTriggerOwned_b56* ptr;
    };

    UnkOwnedTrigger_la63ae4(UnkOwnedTrigger_la63ae4&& other) {
        UnkTriggerOwned_b56* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkOwnedTrigger_la63ae4() {
        UnkTriggerOwned_b56* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_7100a632e8(p);
            fn_7100083c00(p);
        }
    }
};

struct UnkOwnedTriggerArray_la63ae4 {
    UnkOwnedTrigger_la63ae4* data;
    u32 count;
};

void fn_7100a63ae4(UnkOwnedTriggerArray_la63ae4* self, u32* index) {
    u64 i = *index;
    if (self->count > i) {
        self->data[i].~UnkOwnedTrigger_la63ae4();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedTrigger_la63ae4* cur = &self->data[i];
            new (cur) UnkOwnedTrigger_la63ae4(static_cast<UnkOwnedTrigger_la63ae4&&>(cur[1]));
            (&self->data[i])[1].~UnkOwnedTrigger_la63ae4();
        }
        self->count = self->count - 1;
    }
}
