// tools/tu_map.py units 275, 283, 284 and 285 (no class hint): one setter on three sibling classes whose
// fields sit at different offsets: stores `value` twice and `limit`, takes `fallback` or (when it is
// negative) the stored default, keeps `flag`, and on `refresh` calls vtable slot 10 with true.
// Names are placeholders.
#include "types.h"

struct UnkRangeVirtuals275 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10(bool on);
};

template <u32 Base, u32 FlagOffset>
struct UnkRange275 : UnkRangeVirtuals275 {
    u8 pad_0x8[Base - 0x8];
    f32 default_value;  // Base
    f32 current;        // Base + 0x4
    f32 start;          // Base + 0x8
    f32 fallback;       // Base + 0xc
    f32 limit;          // Base + 0x10
    u8 pad_end[FlagOffset - 0x14];
    bool flag;  // Base + FlagOffset
};

template <u32 Base, u32 FlagOffset>
static inline void SetRange(UnkRange275<Base, FlagOffset>* self, f32 value, f32 limit, f32 fallback, bool refresh,
                            bool flag) {
    self->start = value;
    self->current = value;
    self->limit = limit;
    if (fallback < 0.0f) {
        fallback = self->default_value;
    }
    self->fallback = fallback;
    self->flag = flag;
    if (refresh) {
        self->vf_10(true);
    }
}

void fn_71001f6cf0(UnkRange275<0x160, 0x20>* self, f32 value, f32 limit, f32 fallback, bool refresh, bool flag) {
    SetRange(self, value, limit, fallback, refresh, flag);
}

void fn_71002078e4(UnkRange275<0x114, 0x18>* self, f32 value, f32 limit, f32 fallback, bool refresh, bool flag) {
    SetRange(self, value, limit, fallback, refresh, flag);
}

void fn_7100208d1c(UnkRange275<0x118, 0x18>* self, f32 value, f32 limit, f32 fallback, bool refresh, bool flag) {
    SetRange(self, value, limit, fallback, refresh, flag);
}

// Unit 283 holds the same setter with the default taken from the part at +0x108 (+0xc4).
struct UnkRangePart283 {
    u8 pad_0x0[0xc4];
    f32 default_value;  // 0xc4
};

struct UnkRange283 : UnkRangeVirtuals275 {
    u8 pad_0x8[0x100];
    UnkRangePart283* part;  // 0x108
    u8 pad_0x110[0x4];
    f32 current;   // 0x114
    f32 start;     // 0x118
    f32 fallback;  // 0x11c
    f32 limit;     // 0x120
    u8 pad_0x124[0x4];
    bool flag;  // 0x128
};

void fn_7100206fac(UnkRange283* self, f32 value, f32 limit, f32 fallback, bool refresh, bool flag) {
    self->start = value;
    self->current = value;
    self->limit = limit;
    if (fallback < 0.0f) {
        fallback = self->part->default_value;
    }
    self->fallback = fallback;
    self->flag = flag;
    if (refresh) {
        self->vf_10(true);
    }
}
