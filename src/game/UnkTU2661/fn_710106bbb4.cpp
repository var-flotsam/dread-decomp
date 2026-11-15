// Unit 2661 (CCutscene::CTunableCutscene): calls virtual slot 0x188 on the object each entry of the list at +0x150 resolves to.
// Names and layouts are placeholders.
#include "types.h"

struct UnkTarget2661 {
    void** vtable;
};

struct UnkEntry2661 {
    u8 pad_0x0[0x18];
    void* link;  // 0x18
    u8 pad_0x20[0x10];
};

struct UnkOwner2661 {
    u8 pad_0x0[0x150];
    UnkEntry2661* entries;  // 0x150
    u32 count;              // 0x158
};

UnkTarget2661* fn_710106bc20(void* link);

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710106bbb4(UnkOwner2661* self) {
    UnkEntry2661* it = self->entries;
    UnkEntry2661* end = it + self->count;
    for (; it != end; ++it) {
        UnkTarget2661* target = fn_710106bc20(it->link);
        if (target)
            reinterpret_cast<void (*)(UnkTarget2661*)>(target->vtable[0x188 / 8])(target);
    }
}
