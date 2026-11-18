// UnkTU1370 (tools/tu_map.py unit 1370): the copy constructor of the damage request that
// fn_71008c0c74 constructs: the default state, then the value at +0xf8 (rounded to hundredths, as
// fn_71008c1538 sets it), then every other member copied from `other`. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

extern void* unk_7101a7e178[];  // vtable (reached through the GOT)

// A 16-byte value copied as a whole.
struct UnkPair_l8c1090 {
    u64 a;
    u64 b;
};

struct UnkDamageRequest_l8c1090 {
    void** vtable;
    base::UnkRefHandle field_0x8;
    base::UnkRefHandle field_0x10;
    bool field_0x18;
    u32 field_0x1c;
    bool field_0x20;
    bool field_0x21;
    bool field_0x22;
    bool field_0x23;
    bool field_0x24;
    bool field_0x25;
    bool field_0x26;
    bool field_0x27;
    bool field_0x28;
    bool field_0x29;
    bool field_0x2a;
    u32 field_0x2c;
    bool field_0x30;
    UnkPair_l8c1090 field_0x38;
    bool field_0x48;
    bool field_0x49;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u64 field_0x60;
    s32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u32 field_0x94;
    u64 field_0x98;
    CStrId field_0xa0;
    CStrId field_0xa8;
    u32 field_0xb0;
    u32 field_0xb4;
    CStrId field_0xb8;
    u32 field_0xc0;
    u32 field_0xc4;
    u32 field_0xc8;
    bool field_0xcc;
    u32 field_0xd0;
    bool field_0xd4;
    bool field_0xd5;
    u64 field_0xd8;
    u64 field_0xe0;
    u64 field_0xe8;
    u8 pad_0xf0[0x4];
    bool field_0xf4;
    bool field_0xf5;
    bool field_0xf6;
    f32 field_0xf8;

    // fn_71008c1538: keeps the value rounded towards zero to hundredths.
    void SetValue(f32 value) { field_0xf8 = (s32)(value * 100.0f) / 100.0f; }
};
static_assert(__builtin_offsetof(UnkDamageRequest_l8c1090, field_0x38) == 0x38, "layout");
static_assert(__builtin_offsetof(UnkDamageRequest_l8c1090, field_0x60) == 0x60, "layout");
static_assert(__builtin_offsetof(UnkDamageRequest_l8c1090, field_0xa0) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkDamageRequest_l8c1090, field_0xd8) == 0xd8, "layout");
static_assert(__builtin_offsetof(UnkDamageRequest_l8c1090, field_0xf8) == 0xf8, "layout");

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_71008c1090(UnkGooplotRequest_b63a3* self_, const UnkGooplotRequest_b63a3* other_) {
    auto* self = reinterpret_cast<UnkDamageRequest_l8c1090*>(self_);
    auto* other = reinterpret_cast<const UnkDamageRequest_l8c1090*>(other_);

    self->vtable = &unk_7101a7e178[2];
    self->field_0x8.ptr = nullptr;
    self->field_0x10.ptr = nullptr;
    self->field_0x38.a = 0;
    self->field_0x38.b = 0;
    self->field_0x4c = 0;
    self->field_0x50 = 0;
    self->field_0x54 = 0;
    self->field_0x58 = 0;
    self->field_0x68 = -1;
    self->field_0x6c = 0;
    self->field_0x70 = 0;
    self->field_0x74 = 0;
    self->field_0x78 = 0;
    self->field_0x7c = 0;
    self->field_0x80 = 0;
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x8c = 0;
    self->field_0x90 = 0;
    self->field_0x94 = 0;
    new (&self->field_0xa0) CStrId(base::global::CStrId::Empty());
    InitEmptyStrId(&self->field_0xa8);
    InitEmptyStrId(&self->field_0xb8);
    self->field_0xd8 = 0;
    self->field_0xe0 = 0;
    self->field_0xe8 = 0;

    self->SetValue(other->field_0xf8);
    fn_710031d208(&self->field_0x8, &other->field_0x8);
    fn_710031d208(&self->field_0x10, &other->field_0x10);
    self->field_0x18 = other->field_0x18;
    self->field_0x21 = other->field_0x21;
    self->field_0x22 = other->field_0x22;
    self->field_0x23 = other->field_0x23;
    self->field_0x24 = other->field_0x24;
    self->field_0x25 = other->field_0x25;
    self->field_0x26 = other->field_0x26;
    self->field_0x27 = other->field_0x27;
    self->field_0x28 = other->field_0x28;
    self->field_0x29 = other->field_0x29;
    self->field_0x38 = other->field_0x38;
    self->field_0x48 = other->field_0x48;
    self->field_0x1c = other->field_0x1c;
    self->field_0x20 = other->field_0x20;
    self->field_0x68 = other->field_0x68;
    self->field_0x6c = other->field_0x6c;
    self->field_0x70 = other->field_0x70;
    self->field_0x74 = other->field_0x74;
    self->field_0x78 = other->field_0x78;
    self->field_0x7c = other->field_0x7c;
    self->field_0x80 = other->field_0x80;
    self->field_0x84 = other->field_0x84;
    self->field_0x88 = other->field_0x88;
    self->field_0x8c = other->field_0x8c;
    self->field_0x90 = other->field_0x90;
    self->field_0x94 = other->field_0x94;
    self->field_0xa0 = other->field_0xa0;
    self->field_0xa8 = other->field_0xa8;
    self->field_0xb8 = other->field_0xb8;
    self->field_0xb0 = other->field_0xb0;
    self->field_0xb4 = other->field_0xb4;
    self->field_0xc8 = other->field_0xc8;
    self->field_0xcc = other->field_0xcc;
    self->field_0x60 = other->field_0x60;
    self->field_0x98 = other->field_0x98;
    self->field_0xc0 = other->field_0xc0;
    self->field_0xc4 = other->field_0xc4;
    self->field_0x49 = other->field_0x49;
    self->field_0x4c = other->field_0x4c;
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    self->field_0x58 = other->field_0x58;
    self->field_0x2a = other->field_0x2a;
    self->field_0xd0 = other->field_0xd0;
    self->field_0xd4 = other->field_0xd4;
    self->field_0xd5 = other->field_0xd5;
    self->field_0x2c = other->field_0x2c;
    self->field_0xf4 = other->field_0xf4;
    self->field_0xf5 = other->field_0xf5;
    self->field_0xf6 = other->field_0xf6;
    self->field_0x30 = other->field_0x30;
}
