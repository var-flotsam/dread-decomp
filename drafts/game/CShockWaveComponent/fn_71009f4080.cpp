// CShockWaveComponent unit (tools/tu_map.py unit 1505): its update. Advances the wave's timer and radius,
// follows a model joint with the owner while the wave runs, moves the owner outward and ends it at the
// limits, spawns a ring of effects along the wave as it grows, and counts the cooldown down. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CComponent.h"
#include "game/CFXComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkShockOwner_l9f4080 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x3b0 - 0x94];
    u8 field_0x3b0;
    u8 pad_0x3b1[0xf];
    u8 field_0x3c0;
};
static_assert(__builtin_offsetof(UnkShockOwner_l9f4080, field_0x3c0) == 0x3c0, "");

struct UnkShockFxHost_l9f4080 {
    u8 pad_0x0[0x228];
    UnkEntity* model;  // 0x228
    u8 pad_0x230[0x50];
    char* fxObject;  // 0x280
};

struct UnkShockHolder_l9f4080 {
    u8 pad_0x0[0x10];
    UnkShockFxHost_l9f4080* host;  // 0x10
};

struct UnkShockFx_l9f4080 {
    void** vtable;
    void vf_0x18(const void* v) { reinterpret_cast<void (*)(UnkShockFx_l9f4080*, const void*)>(vtable[0x18 / 8])(this, v); }
    void vf_0x20(const void* v) { reinterpret_cast<void (*)(UnkShockFx_l9f4080*, const void*)>(vtable[0x20 / 8])(this, v); }
    void vf_0x40(u32 on) { reinterpret_cast<void (*)(UnkShockFx_l9f4080*, u32)>(vtable[0x40 / 8])(this, on); }
};

struct UnkShockWave_l9f4080 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkShockOwner_l9f4080* owner;  // 0x30
    u8 pad_0x38[0x164 - 0x38 - 0x60 + 0x60];
    f32 maxRadius;  // 0x164
    u32 field_0x168;
    u8 pad_0x16c[8];
    f32 delay;  // 0x174
    bool field_0x178;
    u8 pad_0x179[7];
    CStrId joint;  // 0x180
    bool followPos;   // 0x188
    bool followAngle;  // 0x189
    u8 pad_0x18a[0x1c8 - 0x18a];
    UnkShockHolder_l9f4080* holder;  // 0x1c8
    u8 pad_0x1d0[0x208 - 0x1d0];
    f32 push;  // 0x208
    f32 pushScale;  // 0x20c
    f32 limitHigh;  // 0x210
    f32 limitLow;   // 0x214
    base::global::CFilePathStrId effect;  // 0x218
    f32 interval;  // 0x220
    base::math::CVector3D offsetA;  // 0x224
    base::math::CVector3D offsetB;  // 0x230
    u8 pad_0x23c[0x280 - 0x23c];
    f32 time;    // 0x280
    f32 radius;  // 0x284
    f32 speed;   // 0x288
    u32 field_0x28c;
    u8 pad_0x290[2];
    bool field_0x292;
    u8 pad_0x293;
    s32 spawned;  // 0x294
    f32 cooldown;  // 0x298

    UnkVec3f vf_0x170() { return reinterpret_cast<UnkVec3f (*)(UnkShockWave_l9f4080*)>(vtable[0x170 / 8])(this); }
};

void fn_71009f4080(UnkShockWave_l9f4080* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    self->time += dt;
    if (!(self->radius < self->maxRadius) || (self->field_0x292 && self->field_0x178)) {
        if (self->delay >= 0.0f) {
            self->delay -= dt;
            if (self->delay <= 0.0f) {
                self->delay = 0.0f;
                fn_7100232c68(self->owner);
            }
        }
    } else {
        f32 r = self->time * self->speed;
        self->field_0x28c = self->field_0x168;
        self->radius = r < self->maxRadius ? r : self->maxRadius;
    }

    if (self->joint.data != &g_unk_7101d47108->empty) {
        UnkShockFxHost_l9f4080* host = nullptr;
        if ((self->followPos || self->followAngle) && self->holder && (host = self->holder->host) && host->model) {
            u64 key = self->joint.data->Key();
            UnkMatrix44 matrix;
            for (int k = 0; k < 16; ++k)
                matrix.m[k] = 0.0f;
            matrix.m[15] = 1.0f;
            matrix.m[10] = 1.0f;
            matrix.m[5] = 1.0f;
            matrix.m[0] = 1.0f;
            fn_71011735e8(host->model, &key, &matrix, 0);
            if (self->followPos) {
                UnkShockOwner_l9f4080* owner = self->owner;
                owner->xform.SetTranslation(UnkVec3f(matrix.m[3], matrix.m[7], matrix.m[11]));
                owner->field_0x3b0 = 1;
            }
            if (self->followAngle) {
                UnkVec3f angle = fn_7100076acc(reinterpret_cast<const UnkXform*>(&matrix));
                UnkShockOwner_l9f4080* owner = self->owner;
                owner->xform.SetAngle(angle);
                owner->field_0x3c0 = 1;
            }
        }
    }

    if (self->push > 0.0f && self->radius >= self->maxRadius) {
        f32 step = self->pushScale * dt;
        UnkShockOwner_l9f4080* owner = self->owner;
        f32 x = owner->xform.m[0][3];
        owner->field_0x3b0 = 1;
        f32 nx = x + self->push * step;
        owner->xform.m[0][3] = nx;
        if (nx < self->limitLow || nx > self->limitHigh)
            fn_7100232c68(self->owner);
    }

    if (self->effect.data != &g_unk_7101d47200->empty && self->holder && self->holder->host) {
        char* fxObject = self->holder->host->fxObject;
        if (fxObject) {
            UnkVec3f dir = self->vf_0x170();
            if (self->radius >= 0.0f) {
                f32 t = 0.0f;
                for (s32 i = 0;; ++i) {
                    if (i >= self->spawned) {
                        UnkShockOwner_l9f4080* owner = self->owner;
                        f32 dx = dir.x, dy = dir.y, dz = dir.z;
                        f32 px = owner->xform.m[0][3], py = owner->xform.m[1][3], pz = owner->xform.m[2][3];
                        char* fx = fn_71011520a8(fxObject, 0, reinterpret_cast<char*>(&self->effect), 1);
                        if (fx) {
                            base::math::CVector3D offset = {px + t * dx, py + t * dy, pz + t * dz};
                            base::math::CVector3D rotation = {0.0f, 0.0f, 0.0f};
                            fn_710115217c(fxObject, fx, 0, 0, &CStrId::Empty(), offset, rotation, nullptr);
                            fn_710115596c(reinterpret_cast<UnkRinkaControl_ruc*>(fxObject), fx);
                            UnkShockFx_l9f4080* obj = reinterpret_cast<UnkShockFx_l9f4080*>(fx);
                            obj->vf_0x18(&self->offsetA);
                            obj->vf_0x20(&self->offsetB);
                            obj->vf_0x40(1);
                            self->spawned = i + 1;
                        }
                    }
                    t += self->interval;
                    if (t > self->radius)
                        break;
                }
            }
        }
    }

    if (self->cooldown > 0.0f) {
        self->cooldown -= dt;
        if (self->cooldown < 0.0f)
            self->cooldown = 0.0f;
    }
}
