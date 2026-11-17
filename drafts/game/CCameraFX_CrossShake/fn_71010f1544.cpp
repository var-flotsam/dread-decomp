// CCameraFX_CrossShake's unit (tools/tu_map.py unit 2697): moves the camera position around a cross
// shape (a square with its diagonals) at the shake's phase, sideways and up from the view direction,
// then runs the base shake. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCameraShake_ovb0108;
struct UnkShakePos_ovb0108;
f32 fn_71010f3f20(UnkCameraShake_ovb0108* self, const UnkShakePos_ovb0108* pos, f32 scale);

struct UnkCrossShake_l0f1544 {
    u8 pad_0x0[0x34];
    f32 duration;  // 0x34
    u8 pad_0x38[0x18];
    f32 time;          // 0x50
    bool done;         // 0x54
    u8 pad_0x55[3];
    f32 amplitude;     // 0x58
    f32 cycles;        // 0x5c
};

void fn_71010f3eb0(UnkCrossShake_l0f1544* self, void* camera, f32 dt, CVector3D* pos, CVector3D* target, void* a4,
                   void* a5);

static inline CVector3D Cross(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    CVector3D r;
    r.x = ay * bz - az * by;
    r.y = az * bx - ax * bz;
    r.z = ax * by - ay * bx;
    return r;
}

void fn_71010f1544(UnkCrossShake_l0f1544* self, void* camera, f32 dt, CVector3D* pos, const CVector3D* dir, void* a4,
                   void* a5) {
    if (!self->done) {
        f32 phase = self->time / (self->duration / self->cycles);
        f32 t = fn_71010f3f20(reinterpret_cast<UnkCameraShake_ovb0108*>(self),
                              reinterpret_cast<const UnkShakePos_ovb0108*>(pos), phase - (f32)(s32)phase);
        // Along the path: four edges of lengths 1, sqrt 2, 1, sqrt 2.
        t = t * 4.8284273f - 0.70710677f;
        if (t < 0.0f)
            t += 4.8284273f;
        f32 x, y;
        if (t <= 1.0f) {
            x = (t - 0.5f) * 2.0f;
            y = 1.0f;
        } else if (t <= 2.4142137f) {
            x = ((t - 1.0f) / 1.4142135f - 0.5f) * 2.0f;
            y = x;
        } else if (t <= 3.4142137f) {
            x = (t - 2.4142137f - 0.5f) * 2.0f;
            y = -1.0f;
        } else {
            x = ((t - 3.4142137f) / 1.4142135f - 0.5f) * 2.0f;
            y = x;
        }
        if (self->amplitude > 0.0f) {
            CVector3D side = Cross(dir->x, dir->y, dir->z, 0.0f, 1.0f, 0.0f);
            CVector3D up;
            up.x = dir->z * side.y - dir->y * side.z;
            up.y = dir->x * side.z - dir->z * side.x;
            up.z = dir->y * side.x - dir->x * side.y;
            f32 amp = self->amplitude;
            CVector3D moved;
            moved.x = pos->x + amp * (x * side.x);
            moved.y = pos->y + amp * (x * side.y);
            moved.z = pos->z + amp * (x * side.z);
            *pos = moved;
            amp = self->amplitude;
            moved.x = pos->x + y * up.x * amp;
            moved.y = pos->y + y * up.y * amp;
            moved.z = pos->z + y * up.z * amp;
            *pos = moved;
        }
    }
    fn_71010f3eb0(self, camera, dt, pos, pos, a4, a5);
}
