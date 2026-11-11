// Functions in the CSamusMovement unit that walk its state list (a u32 count at +0x2d18 followed
// directly by the pointers, at +0x2d1c). Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkHandleOwner_71010c4230;
struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);

struct UnkSamusMoveState_A6_b70 {
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
};

struct UnkSamusMoveTracker_A6_b70 {
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
    u8 pad_0x8[0x70 - 0x8];
    void* field_0x70;
    void* field_0x78;
    void* field_0x80;
};

#pragma pack(push, 4)
struct UnkSamusMove_A6_b70 {
    u8 pad_0x0[0x2d18];
    u32 state_count;
    UnkSamusMoveState_A6_b70* states[1];  // 0x2d1c, 4-byte aligned
    u8 pad_0x2d24[0x5240 - 0x2d24];
    UnkLinkList* link;
    u8 pad_0x5248[0x5260 - 0x5248];
    u8 timer[0x5468 - 0x5260];
    UnkSamusMoveTracker_A6_b70* tracker_a;
    UnkSamusMoveTracker_A6_b70* tracker_b;
};
#pragma pack(pop)

void fn_71009e0658(UnkSamusMove_A6_b70* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    fn_710002fa90(self->timer);
    self->tracker_a->vf_0x80();
    self->tracker_b->vf_0x80();
    self->tracker_a->field_0x70 = nullptr;
    self->tracker_a->field_0x78 = nullptr;
    self->tracker_a->field_0x80 = nullptr;
    self->tracker_b->field_0x70 = nullptr;
    self->tracker_b->field_0x78 = nullptr;
    self->tracker_b->field_0x80 = nullptr;
    UnkSamusMoveState_A6_b70** it = self->states;
    u64 bytes = (u64)self->state_count * sizeof(*it);
    asm("" : "+r"(bytes));
    for (; bytes != 0; ++it, bytes -= sizeof(*it))
        (*it)->vf_0x50();
    if (self->link) {
        fn_71002aeb9c(self->link, &self->link);
        self->link = nullptr;
    }
}

// A second view of the movement for the state reset below: the fields it clears.
struct UnkSamusMoveTunables_A6_b70 {
    u8 pad_0x0[0xc4];
    f32 field_0xc4;
};

struct UnkSamusMoveStateB_A6_b70 {
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
};

struct UnkVec3_A6_b70 {
    f32 x, y, z;
};

#pragma pack(push, 4)
struct UnkSamusMoveB_A6_b70 {
    void** vtable;
    u8 pad_0x8[0xf9 - 0x8];
    bool pending;
    u8 pad_0xfa[0x1d8 - 0xfa];
    UnkVec3_A6_b70 field_0x1d8;
    u8 pad_0x1e4[0x7c8 - 0x1e4];
    f32 field_0x7c8;
    u8 pad_0x7cc[0x2290 - 0x7cc];
    f32 field_0x2290;
    u8 pad_0x2294[0x2298 - 0x2294];
    f32 field_0x2298;
    u8 pad_0x229c[0x2314 - 0x229c];
    u64 field_0x2314;
    u8 pad_0x231c[0x2320 - 0x231c];
    u32 field_0x2320;
    u16 field_0x2324;
    u8 pad_0x2326[0x2330 - 0x2326];
    UnkVec3_A6_b70 field_0x2330;
    u8 pad_0x233c[0x2340 - 0x233c];
    f32 field_0x2340;
    u8 pad_0x2344[0x2380 - 0x2344];
    UnkVec3_A6_b70 field_0x2380;
    u8 pad_0x238c[0x2d18 - 0x238c];
    u32 state_count;
    UnkSamusMoveStateB_A6_b70* states[1];  // 0x2d1c, 4-byte aligned
    u8 pad_0x2d24[0x5458 - 0x2d24];
    UnkSamusMoveTunables_A6_b70* tunables;
};
#pragma pack(pop)

void fn_710097873c(UnkSamusMoveB_A6_b70* self);

void fn_71009e9a04(UnkSamusMoveB_A6_b70* self) {
    u16* flags = &self->field_0x2324;
    asm("" : "+r"(flags));  // the original computes this address before the call
    fn_710097873c(self);
    self->field_0x1d8 = {};
    self->field_0x2380 = {};
    if (self->pending) {
        self->pending = false;
        reinterpret_cast<void (*)(UnkSamusMoveB_A6_b70*)>(self->vtable[0x260 / 8])(self);
    }
    self->field_0x2340 = 0;
    self->field_0x2314 = 0;
    self->field_0x2320 = 0;
    self->field_0x7c8 = 0;
    self->field_0x2290 = self->tunables->field_0xc4;
    self->field_0x2298 = -1.0f;
    *flags = 0;
    UnkSamusMoveStateB_A6_b70** begin = self->states;
    u64 bytes = (u64)self->state_count * sizeof(*begin);
    asm("" : "+r"(bytes));
    self->field_0x2330 = {};
    if (bytes == 0)
        return;
    asm("" : "+r"(begin));
    UnkSamusMoveStateB_A6_b70** it = begin;
    do {
        (*it)->vf_0xb0();
        ++it;
    } while (it != begin + self->state_count);
}
