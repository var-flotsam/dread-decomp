// Passes the pair on to fn_71002c46d4 unless the first object's target has a tracked entry (id
// words not -1 and not 0xff) whose value at 0x4 lies more than 20 away from the first
// object's value at 0x44; then the answer is 0. The entry is returned by vf_0x2d8 and destroyed
// afterwards.
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

u32 fn_71002c46d4(void* self, UnkActor* first, void* second);

u32 fn_7100478548(void* self, UnkActor* first, void* second) {
    UnkActor* owner = fn_7100d7c990(first);
    if (owner && owner->field_0x1f8) {
        f32 x = first->field_0x44;
        UnkEntry entry = static_cast<UnkHolder*>(owner->field_0x1f8)->vf_0x2d8();
        if (entry.field_0x0 != -1 && entry.field_0x2 != 0xff) {
            f32 value = entry.field_0x4;
            if (!(value + -20.0f <= x) || !(value + 20.0f >= x))
                return 0;
        }
    }
    return fn_71002c46d4(self, first, second);
}
