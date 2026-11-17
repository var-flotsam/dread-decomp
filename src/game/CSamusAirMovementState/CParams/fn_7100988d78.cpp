// CSamusAirMovementState::CParams's unit (tools/tu_map.py unit 1455): a constructor. The movement-state
// params base (fn_71009b06dc), then this class's vtable and its own members cleared, an id of -1 and the
// base word at +0x10 set to 1. Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "types.h"

struct UnkSamusParams_l9b06dc;
void fn_71009b06dc(UnkSamusParams_l9b06dc* self);

extern char g_unk_7101a862e0[];

// A record at +0x1c0: two cleared words, an id of -1 and four cleared vectors.
struct UnkAirRecord_l988d78 {
    u64 field_0x0;
    u64 field_0x8;
    s32 id;  // 0x10
    base::math::CVector3D vectors[4];  // 0x14
    u32 field_0x44;

    UnkAirRecord_l988d78() : field_0x0(0), field_0x8(0), id(-1) {
        for (base::math::CVector3D& v : vectors) {
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 0.0f;
        }
    }
};

struct UnkAirParams_l988d78 {
    char* vtable;
    u64 field_0x8;
    u32 field_0x10;
    u8 pad_0x14[0x174];
    u64 field_0x188;
    u64 field_0x190;
    u64 field_0x198;
    bool field_0x1a0;
    u32 field_0x1a4[6];
    u8 pad_0x1bc[4];
    UnkAirRecord_l988d78 record;  // 0x1c0
    bool field_0x208;
    u32 field_0x20c;
    bool field_0x210;
};
static_assert(__builtin_offsetof(UnkAirParams_l988d78, field_0x188) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkAirParams_l988d78, record) == 0x1c0, "layout");
static_assert(__builtin_offsetof(UnkAirParams_l988d78, field_0x210) == 0x210, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100988d78(UnkAirParams_l988d78* self) {
    fn_71009b06dc(reinterpret_cast<UnkSamusParams_l9b06dc*>(self));
    self->vtable = g_unk_7101a862e0 + 0x10;
    self->field_0x188 = 0;
    self->field_0x190 = 0;
    self->field_0x198 = 0;
    self->field_0x1a0 = false;
    for (u32& v : self->field_0x1a4)
        v = 0;
    new (&self->record) UnkAirRecord_l988d78();
    self->field_0x208 = false;
    self->field_0x20c = 0;
    self->field_0x210 = false;
    self->field_0x10 = 1;
}
