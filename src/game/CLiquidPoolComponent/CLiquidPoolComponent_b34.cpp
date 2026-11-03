#include "types.h"

struct UnkLiquidPoolTarget {
    void** vtable;
};

struct CLiquidPoolComponent {
    u8 unk_0x0[0x88];
    UnkLiquidPoolTarget* target;
};

// Passes 0x1090 or 0x1080 (by `flag`) to the target's slot 0x60, when there is a target.
void fn_7100902a94(CLiquidPoolComponent* self, bool flag) {
    if (UnkLiquidPoolTarget* t = self->target)
        reinterpret_cast<void (*)(UnkLiquidPoolTarget*, u32)>(t->vtable[0x60 / 8])(t, flag ? 0x1090 : 0x1080);
}

struct UnkLiquidPoolSelf {
    void** vtable;
    u8 unk_0x8[0xc11 - 0x8];
    bool field_0xc11;
};

// Calls slot 0x208 with `force` or, when not forced, the flag at 0xc11.
bool fn_71009025d8(UnkLiquidPoolSelf* self, bool force) {
    bool value = true;
    if (!force)
        value = self->field_0xc11;
    reinterpret_cast<void (*)(UnkLiquidPoolSelf*, bool)>(self->vtable[0x208 / 8])(self, value);
    return false;
}
