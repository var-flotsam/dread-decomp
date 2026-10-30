// Whether another object stands between this object and its target: false without a position
// object; otherwise, over the registered objects other than this one (looked up by key through the
// global at 0x7101d47310), true for one at nearly the same height (within 10) whose x lies between
// the two.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkEntity {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
};

struct UnkEntry {
    u8 pad_0x0[0x10];
    UnkEntity* field_0x10;
};

struct UnkEntryList {
    UnkEntry** data;
    u32 count;

    UnkEntry** begin() { return data; }
    UnkEntry** end() { return data + count; }
};

extern char g_unk_71726cca00[] __attribute__((visibility("hidden")));

// NON_MATCHING(minor): the height band test (+-10) is a conditional-compare chain with its two operands
// swapped, and the two constants are materialised in the other order
bool fn_71004744a4(UnkEntity* self) {
    UnkActor* target = fn_7100d7c990(self->field_0x30);
    if (!target)
        return false;
    UnkEntryList* list = reinterpret_cast<UnkEntryList*>(
        fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(g_unk_71726cca00)));
    for (UnkEntry* entry : *list) {
        UnkEntity* other = entry ? entry->field_0x10 : nullptr;
        if (other == self)
            continue;
        UnkActor* me = self->field_0x30;
        f32 my_y = me->field_0x54;
        f32 other_y = other->field_0x30->field_0x54;
        f32 lo = my_y + -10.0f;
        f32 hi = my_y + 10.0f;
        if (!(lo <= other_y) || !(hi >= other_y))
            continue;
        f32 other_x = other->field_0x30->field_0x44;
        f32 my_x = me->field_0x44;
        f32 target_x = target->field_0x44;
        if (my_x > other_x && other_x > target_x)
            return true;
        if (other_x > my_x && other_x < target_x)
            return true;
    }
    return false;
}
