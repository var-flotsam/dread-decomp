// CAccessPointComponent's unit (tools/tu_map.py): two getters of the same shape (fn_7100a6e788 the
// original). While neither part (+0x110, and +0x118 when set) is in state 2, the value from the
// helper with the flag at +0x590 set, else a field of the settings at +0x698; with one in state 2,
// one of two other settings fields by the flag at +0x91d. Names and layouts are placeholders.
#include "types.h"

struct UnkAccessPointPart_b45a2 {
    u8 pad_0x0[0x10];
    s32 state;
};

// The settings block's entries this unit hands out (their type is unknown here).
struct UnkAccessPointEntry_b45a2 {
    u64 value;
};

struct UnkAccessPointSettings_b45a2 {
    u8 pad_0x0[0x170];
    UnkAccessPointEntry_b45a2 field_0x170;
    UnkAccessPointEntry_b45a2 field_0x178;
    u8 pad_0x180[0x38];
    UnkAccessPointEntry_b45a2 field_0x1b8;
    UnkAccessPointEntry_b45a2 field_0x1c0;
    u8 pad_0x1c8[0x30];
    UnkAccessPointEntry_b45a2 field_0x1f8;
    UnkAccessPointEntry_b45a2 field_0x200;
};

struct UnkAccessPoint_b45a2 {
    u8 pad_0x0[0x110];
    UnkAccessPointPart_b45a2* part;
    UnkAccessPointPart_b45a2* other;
    u8 pad_0x120[0x470];
    bool field_0x590;
    u8 pad_0x591[0x107];
    UnkAccessPointSettings_b45a2* settings;  // 0x698
    u8 pad_0x6a0[0x27d];
    bool field_0x91d;
};

UnkAccessPointEntry_b45a2* fn_7100a9f674(UnkAccessPoint_b45a2* self);
UnkAccessPointEntry_b45a2* fn_7100a9f680(UnkAccessPoint_b45a2* self);

using UnkAccessPointField_b45a2 = UnkAccessPointEntry_b45a2 UnkAccessPointSettings_b45a2::*;

template <UnkAccessPointEntry_b45a2* (*Helper)(UnkAccessPoint_b45a2*), UnkAccessPointField_b45a2 Normal,
          UnkAccessPointField_b45a2 Engaged, UnkAccessPointField_b45a2 EngagedAlt>
UnkAccessPointEntry_b45a2* UnkAccessPointGet_b45a2(UnkAccessPoint_b45a2* self) {
    if (self->part->state != 2 && (!self->other || self->other->state != 2)) {
        if (self->field_0x590)
            return Helper(self);
        return &(self->settings->*Normal);
    }
    bool alt = self->field_0x91d;
    UnkAccessPointSettings_b45a2* settings = self->settings;
    UnkAccessPointEntry_b45a2* engaged = &(settings->*Engaged);
    asm("" : "+r"(engaged));  // both addresses are formed before the select, as in the original
    return alt ? &(settings->*EngagedAlt) : engaged;
}

UnkAccessPointEntry_b45a2* fn_7100a6e788(UnkAccessPoint_b45a2* self) {
    return UnkAccessPointGet_b45a2<fn_7100a9f674, &UnkAccessPointSettings_b45a2::field_0x1c0,
                                   &UnkAccessPointSettings_b45a2::field_0x200,
                                   &UnkAccessPointSettings_b45a2::field_0x178>(self);
}

UnkAccessPointEntry_b45a2* fn_7100a6e7e0(UnkAccessPoint_b45a2* self) {
    return UnkAccessPointGet_b45a2<fn_7100a9f680, &UnkAccessPointSettings_b45a2::field_0x1b8,
                                   &UnkAccessPointSettings_b45a2::field_0x1f8,
                                   &UnkAccessPointSettings_b45a2::field_0x170>(self);
}
