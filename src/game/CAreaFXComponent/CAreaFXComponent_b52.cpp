// CAreaFXComponent unit: fn_71007ff654, the update step of CAreaFXComponent_b41.cpp's fn_71007ff7bc without
// its last call (it ends with fn_7100802108). The layouts repeat that file's. Names and layouts are
// placeholders.
#include "types.h"

u32 fn_71010cc3a4(void*);

struct UnkFlagHolder_af41 {
    u8 pad_0x0[0x16e6];
    u8 field_0x16e6;
};

struct UnkOwner_af41 {
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
    virtual void vf_0xd0(const f32* pos, bool flag);

    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b0];
    void* field_0x218;
    u8 pad_0x220[0x20];
    UnkFlagHolder_af41* field_0x240;
};

struct UnkSelf_af41 {
    u8 pad_0x0[0x30];
    UnkOwner_af41* owner;
    u8 pad_0x38[0x3ec];
    u8 has_pos;  // 0x424
    u8 pad_0x425[3];
    f32 pos[3];  // 0x428
};

void fn_7100802108(UnkSelf_af41* self);

static inline bool Near_af41(f32 a, f32 b) {
    f32 lo = a - 0.001f;
    f32 hi = a + 0.001f;
    return lo <= b && hi >= b;
}

void fn_71007ff654(UnkSelf_af41* self) {
    if (void* obj = self->owner->field_0x218)
        if (fn_71010cc3a4(obj) == 2)
            return;
    if (self->has_pos) {
        UnkOwner_af41* owner = self->owner;
        if (!(Near_af41(owner->x, self->pos[0]) && Near_af41(owner->y, self->pos[1]) &&
              Near_af41(owner->z, self->pos[2]))) {
            if (UnkFlagHolder_af41* holder = owner->field_0x240)
                *((u8*)holder + 0x16e6) = 1;
            self->owner->vf_0xd0(self->pos, true);
        }
    }
    fn_7100802108(self);
}
