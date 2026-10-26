// Whether the component may act: false when the object at +0x220 of the component's owner (+0x30)
// has the flag with hash 0xb1908e0ac25fa340. Otherwise true when the state at 0x15de8 is 0; when it
// is 1 the handle at 0x15df0 first takes a new value and the state becomes 2. The answer is then
// whether the object found for the handle has its byte at +0x212 other than 1 (true when there is
// no such object).
#include "base/UnkRefHandle.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// The handle the component refers to (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkCounted : base::UnkRefCounted {
    void* field_0x10;
};

// A counted reference; the destructor decrements and tests in one step.
struct UnkRef {
    UnkCounted* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

struct UnkFound {
    u8 pad_0x0[0x212];
    u8 field_0x212;
};

struct UnkOwnerRef {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwnerRef* field_0x30;
    u8 pad_0x38[0x15d88];
    u8 field_0x15dc0[0x28];
    u32 state;
    u32 pad_0x15dec;
    base::UnkRefHandle handle;
};

UnkRef fn_7100353be8(void* from);
UnkFound* fn_71004067a4(void* owner);

bool fn_7100452608(UnkSelf* self) {
    if (self->field_0x30->field_0x220 && fn_710117ae38(self->field_0x30->field_0x220, 0xb1908e0ac25fa340ull, 0))
        return false;
    if (self->state == 0)
        return true;
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->field_0x15dc0);
            // The reference's pointer is the handle's.
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    UnkCounted* handle = static_cast<UnkCounted*>(self->handle.ptr);
    UnkFound* found = fn_71004067a4(handle ? handle->field_0x10 : nullptr);
    if (!found)
        return true;
    return found->field_0x212 != 1;
}
