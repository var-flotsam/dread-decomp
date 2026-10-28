// 1 when the second object's value at 0x54 is above the limit stored in the target's tracked
// entry (id words not -1 and not 0xff), else 0 (also without a target or its holder at 0x1f8).
// The entry is returned by vf_0x2d8 and destroyed afterwards.
#include "game/UnkActor.h"
#include "types.h"

struct UnkEntry;
void fn_710028760c(UnkEntry* entry);  // its destructor

// Not trivially destructible, so it is returned in memory.
struct UnkEntry {
    s16 field_0x0;
    u8 field_0x2;
    u8 pad_0x3;
    f32 field_0x4;
    f32 field_0x8;
    u8 pad_0xc[4];

    UnkEntry() = default;
    ~UnkEntry() { fn_710028760c(this); }
};

struct UnkHolder {
    void** vtable;
    UnkEntry vf_0x2d8() { return reinterpret_cast<UnkEntry (*)(UnkHolder*)>(vtable[0x2d8 / 8])(this); }
};

struct UnkChecked {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

// NON_MATCHING(minor): the failing and succeeding exits of the entry's scope (each destroys it) are laid out
// in the other order, so three branches have the opposite polarity
f32 fn_7100475e54(void* self, UnkActor* first, UnkChecked* second) {
    f32 result = 0.0f;
    UnkActor* owner = fn_7100d7c990(first);
    if (owner && owner->field_0x1f8) {
        bool above = false;
        {
            UnkEntry entry = static_cast<UnkHolder*>(owner->field_0x1f8)->vf_0x2d8();
            if (entry.field_0x0 != -1 && entry.field_0x2 != 0xff && second->field_0x54 > entry.field_0x8)
                above = true;
        }
        if (above)
            result = 1.0f;
    }
    return result;
}
