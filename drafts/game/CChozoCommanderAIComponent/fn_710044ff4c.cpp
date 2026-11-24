// CChozoCommanderAIComponent unit (tools/tu_map.py): runs the base reset, puts the component's state
// block (+0x15cf8: flags, per-step flags, a zero position and a range up to FLT_MAX) back to its
// defaults, then resets three of its attacks. Names and layouts are placeholders.
#include "types.h"

struct UnkObject_71003ad278;
struct UnkChargeAttack2_ovb0113;
struct UnkHypersparkAttack_ovb0023;

void fn_71003ad278(UnkObject_71003ad278* self);
void fn_71004365a8(void* self);
void fn_710042dbfc(UnkHypersparkAttack_ovb0023* self);
void CChozoCommanderAirChargeAttack_ComputeTimelineMotionOffset(UnkChargeAttack2_ovb0113* self);

struct UnkVec3_l44ff4c {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
};

struct UnkChozoState_l44ff4c {
    UnkChozoState_l44ff4c() {}

    u32 field_0x0 = 0;
    u8 pad_0x4[0x8];
    bool field_0xc = false;
    bool field_0xd = true;
    bool field_0xe[0x13] = {};
    u8 pad_0x21[0x19f];
    bool field_0x1c0 = false;
    bool field_0x1c1 = false;
    u8 pad_0x1c2[0x2];
    UnkVec3_l44ff4c field_0x1c4;
    u8 pad_0x1d0[0x8];
    bool field_0x1d8 = false;
    bool field_0x1d9 = true;
    u8 pad_0x1da[0x2];
    f32 field_0x1dc = 0.0f;
    f32 field_0x1e0 = 0.0f;
    f32 field_0x1e4 = 0.0f;
    f32 field_0x1e8 = 3.402823466e+38f;
    u8 pad_0x1ec[0x1c];
    bool field_0x208 = false;
};
static_assert(__builtin_offsetof(UnkChozoState_l44ff4c, field_0x1c0) == 0x1c0, "");
static_assert(__builtin_offsetof(UnkChozoState_l44ff4c, field_0x1d8) == 0x1d8, "");
static_assert(__builtin_offsetof(UnkChozoState_l44ff4c, field_0x208) == 0x208, "");

struct UnkChozoCommanderAI_l44ff4c {
    u8 pad_0x0[0x14a28];
    u8 field_0x14a28[0x2c0];  // attack reset by fn_71004365a8
    u8 field_0x14ce8[0x1d8];  // attack reset by fn_7100425840
    u8 field_0x14ec0[0xe38];  // attack reset by fn_710042dbfc
    UnkChozoState_l44ff4c field_0x15cf8;
};
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l44ff4c, field_0x15cf8) == 0x15cf8, "");

void fn_710044ff4c(UnkChozoCommanderAI_l44ff4c* self) {
    fn_71003ad278(reinterpret_cast<UnkObject_71003ad278*>(self));
    self->field_0x15cf8 = UnkChozoState_l44ff4c();
    fn_71004365a8(self->field_0x14a28);
    fn_710042dbfc(reinterpret_cast<UnkHypersparkAttack_ovb0023*>(self->field_0x14ec0));
    CChozoCommanderAirChargeAttack_ComputeTimelineMotionOffset(reinterpret_cast<UnkChargeAttack2_ovb0113*>(self->field_0x14ce8));
}
