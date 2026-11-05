// CChozoCommanderEnergyShardsFragmentMovementComponent's unit (tools/tu_map.py unit 670): with the movement
// component as second argument, sets its state (+0xc8) to 1, gives its damage request the tunable's value
// (+0xa0 of g_unk_7172716160, fn_71008c1538), copies the unit's static string id into +0xb8, sets bit 26 of
// the flags at +0xb4, then gives the request the direction (+-1, 1, 0) by the sign of the fragment's +0x40
// (+0x4c, fn_710007b444) and flags it (+0x49). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726c9c80 __attribute__((visibility("hidden")));

struct UnkVec3;
void fn_710007b444(void* dst, const UnkVec3* value);

struct UnkDirection_A2_b71 {
    f32 x, y, z;
    UnkDirection_A2_b71(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
};

struct UnkShardsFragment_A2_b71 {
    u8 pad_0x0[0x40];
    f32 field_0x40;
};

struct UnkShardsFragmentMover_A2_b71 {
    u8 pad_0x0[0x49];
    bool field_0x49;
    u8 pad_0x4a[0x2];
    u8 field_0x4c[0x68];
    u32 flags;  // 0xb4
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 state;  // 0xc8
};

void fn_710045984c(UnkShardsFragment_A2_b71* fragment, UnkShardsFragmentMover_A2_b71* self) {
    self->state = 1;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0xa0));
    self->id = g_unk_71726c9c80;
    self->flags |= 0x4000000;
    if (fragment->field_0x40 > 0.0f) {
        UnkDirection_A2_b71 dir(1.0f, 1.0f, 0.0f);
        fn_710007b444(self->field_0x4c, reinterpret_cast<const UnkVec3*>(&dir));
    } else {
        UnkDirection_A2_b71 dir(-1.0f, 1.0f, 0.0f);
        fn_710007b444(self->field_0x4c, reinterpret_cast<const UnkVec3*>(&dir));
    }
    self->field_0x49 = true;
}
