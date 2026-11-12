// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009a9628 derives from a direction the 2D push
// (fn_71009ecf34's sample), the reversed direction, its rotation (fn_710007a7f0) and the id of the animation to
// use (from the game manager's id table while the flag at +0x7e4 is set). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMagnetDirOut_ovb0082 {
    f32 x;
    u32 y;
};

struct UnkMagnetSamus_ovb0082 {
    u8 pad_0x0[0x7e4];
    bool field_0x7e4;
};

struct UnkMagnetDirState_ovb0082 {
    u8 pad_0x0[0x38];
    UnkMagnetSamus_ovb0082* samus;  // 0x38
};

struct UnkMagnetDirSample_ovb0082 {
    f32 x;
    u32 y;
};

void* fn_71009ecf34(UnkMagnetSamus_ovb0082* samus, UnkMagnetDirSample_ovb0082* out, u32 flags);

struct UnkGameManagerIdTableEntry_ovb0082 {
    u8 pad_0x0[0x30];
    u64 hash;
};

void fn_71009a9628(UnkMagnetDirState_ovb0082* self, const UnkVec3f* dir, UnkMagnetDirOut_ovb0082* out_2d,
                   base::math::CVector3D* out_back, UnkVec3f* out_rot, u64* out_id) {
    UnkMagnetDirSample_ovb0082 sample;
    __builtin_memset(&sample, 0, sizeof(sample));
    fn_71009ecf34(self->samus, &sample, 0);
    out_2d->x = -(sample.x * dir->x);
    __builtin_memcpy(&out_2d->y, &sample.y, sizeof(u32));
    *out_back = {-dir->x, -dir->y, -dir->z};
    *out_rot = fn_710007a7f0(UnkVec3f(-dir->x, -dir->y, -dir->z));
    if (self->samus->field_0x7e4)
        *out_id = reinterpret_cast<UnkGameManagerIdTableEntry_ovb0082*>(GameManagerIdTable().field_0x1570.data)->hash;
    else
        *out_id = 0xe89744df5df60463;
}
