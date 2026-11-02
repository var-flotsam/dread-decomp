// From unit 357 of tools/tu_map.py (0x710028659c-0x71002871cc, no reflected class): a 2D size for an
// object: the override at +0x1a4 when the flag at +0x1a0 is set, else what the first of three
// providers of the owner at +0x50 answers (the one at the owner's +0x240, then those fn_7100286ad4
// and fn_7100286bb0 find), else 100 by 100. The size type has a user-provided copy, so it is
// returned in memory (x8). Names and layouts are placeholders.
#include "types.h"

struct UnkSize2f {
    f32 x, y;
    UnkSize2f(f32 x, f32 y) : x(x), y(y) {}
    UnkSize2f(const UnkSize2f& o) : x(o.x), y(o.y) {}
};

// The providers' getters (vf_0x180, vf_0x1c0 and vf_0x230 of three different classes).
struct UnkSizeProviderA {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual UnkSize2f vf_0x180(u32 arg);
};

struct UnkSizeProviderB {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual UnkSize2f vf_0x1c0(u32 arg);
};

struct UnkSizeProviderC : UnkSizeProviderB {
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual UnkSize2f vf_0x230(u32 arg);
};

struct UnkSizeOwner {
    u8 pad_0x0[0x240];
    UnkSizeProviderA* provider;  // 0x240
};

// Declared as its other caller declares it (src/game/CGunComponent/fn_71008397f4.cpp); it takes the owner.
struct UnkWeakTarget;
UnkWeakTarget* fn_7100286ad4();
UnkSizeProviderC* fn_7100286bb0(UnkSizeOwner* owner);

struct UnkUnit357Object {
    u8 pad_0x0[0x50];
    UnkSizeOwner* owner;  // 0x50
    u8 pad_0x58[0x1a0 - 0x58];
    bool has_size;  // 0x1a0
    u8 pad_0x1a1[3];
    UnkSize2f size;  // 0x1a4
};

UnkSize2f fn_7100286a0c(UnkUnit357Object* self, u32 arg) {
    if (self->has_size)
        return self->size;
    if (self->owner) {
        UnkSizeProviderA* a = self->owner->provider;
        UnkSizeProviderB* b = reinterpret_cast<UnkSizeProviderB* (*)(UnkSizeOwner*)>(fn_7100286ad4)(self->owner);
        UnkSizeProviderC* c = fn_7100286bb0(self->owner);
        if (a)
            return a->vf_0x180(arg);
        if (b)
            return b->vf_0x1c0(arg);
        if (c)
            return c->vf_0x230(arg);
    }
    return UnkSize2f(100.0f, 100.0f);
}
