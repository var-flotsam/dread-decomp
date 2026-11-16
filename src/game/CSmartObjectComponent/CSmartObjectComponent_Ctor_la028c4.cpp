// CSmartObjectComponent unit (tools/tu_map.py unit 1512): the component's constructor (destroyed by
// fn_7100a02c54): the base component, empty string ids, unbounded ranges (+-FLT_MAX) and guarded
// zeros. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern void* g_unk_7101a8ad98[];

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkSmartObjectStaticZero {
    u32 value;
    UnkSmartObjectStaticZero() : value(0) {}
};

inline const UnkSmartObjectStaticZero& UnkSmartObjectZero() {
    static UnkSmartObjectStaticZero s_value asm("g_unk_7172716dcc");
    return s_value;
}

static constexpr f32 kMax = 3.40282347e+38f;
// FLT_MAX's bits: some fields are written as words (the original's constant stores of them are
// integer stores).
static constexpr u32 kMaxBits = 0x7f7fffff;

struct UnkSmartObjectComponent_la028c4 {
    void** vtable;
    u8 unk_0x8[0x38];
    f32 field_0x40;
    u8 unk_0x44[0x4];
    CStrId ids[11];             // 0x48
    UnkStringData* field_0xa0;  // CStrId
    bool field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
    u32 field_0xc0;
    CStrId field_0xc8;
    CStrId field_0xd0;
    UnkStringData* field_0xd8;  // CStrId
    bool field_0xe0;
    u32 field_0xe4;
    bool field_0xe8;
    bool field_0xe9;
    bool field_0xea;
    bool field_0xeb;
    bool field_0xec;
    f32 field_0xf0;
    f32 field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
    f32 field_0x100;
    f32 field_0x104;
    u32 field_0x108;  // FLT_MAX's bits
    u8 unk_0x10c[0x8];
    u32 field_0x114;
    u32 field_0x118;
    f32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
    u32 field_0x128;  // FLT_MAX's bits
    u32 field_0x12c;  // FLT_MAX's bits
    u32 field_0x130;  // FLT_MAX's bits
    u8 unk_0x134[0x8];
    u32 field_0x13c;
    u32 field_0x140;
};
static_assert(__builtin_offsetof(UnkSmartObjectComponent_la028c4, field_0xa0) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkSmartObjectComponent_la028c4, field_0xe4) == 0xe4, "layout");
static_assert(__builtin_offsetof(UnkSmartObjectComponent_la028c4, field_0x140) == 0x140, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100a028c4(void* p) {
    auto* self = static_cast<UnkSmartObjectComponent_la028c4*>(p);
    fn_71010c4148(static_cast<UnkStateObject*>(p));
    self->vtable = g_unk_7101a8ad98 + 2;
    self->field_0x40 = 1.0f;
    for (CStrId& id : self->ids)
        new (&id) CStrId(g_unk_710162e2ed, true);
    MakeEmpty(&self->field_0xa0);
    self->field_0xa8 = true;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = 1;
    new (&self->field_0xc8) CStrId(g_unk_710162e2ed, true);
    new (&self->field_0xd0) CStrId(g_unk_710162e2ed, true);
    MakeEmpty(&self->field_0xd8);
    self->field_0xe0 = false;
    self->field_0xe4 = 0;
    self->field_0xe8 = false;
    self->field_0xe9 = false;
    self->field_0xea = true;
    self->field_0xeb = true;
    self->field_0xec = true;
    self->field_0xf0 = 0.0f;
    self->field_0xf4 = -kMax;
    self->field_0xf8 = -kMax;
    self->field_0xfc = -kMax;
    self->field_0x100 = kMax;
    self->field_0x104 = kMax;
    self->field_0x108 = kMaxBits;
    self->field_0x114 = UnkSmartObjectZero().value;
    self->field_0x118 = UnkSmartObjectZero().value;
    self->field_0x11c = -kMax;
    self->field_0x120 = -kMax;
    self->field_0x124 = -kMax;
    self->field_0x128 = kMaxBits;
    self->field_0x12c = kMaxBits;
    self->field_0x130 = kMaxBits;
    self->field_0x13c = UnkSmartObjectZero().value;
    self->field_0x140 = UnkSmartObjectZero().value;
}
