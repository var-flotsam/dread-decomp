// Unit 2573: (re)launches a shot. It takes the shot manager's speed, drops its old references, takes weak
// references to the shooter and the target, sets its flags from the launch parameters, its velocity
// along its direction, the direction's angle, its rotation and a random spin, then clears its state.
// Names and layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CAbilityEnergyWave.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

void* fn_7100ed77a0(void* player);

// The shot manager's tunables: the shot speed at +0x22b8.
struct UnkStruct_71729cf280 {
    u8 pad_0x0[0x22b8];
    f32 field_0x22b8;
};
extern UnkStruct_71729cf280 g_unk_71729cf280;

// A counted reference released on destruction.
struct UnkShotRef_lecc424 {
    UnkRefObject* p;
    ~UnkShotRef_lecc424() {
        if (UnkRefObject* r = p) {
            if (--r->ref_count == 0)
                r->vf_0x10();
        }
    }
};

struct UnkShotRecord18_lecc424 {
    UnkShotRef_lecc424 ref;
    u8 pad_0x8[0x10];
};

struct UnkShotRecord10_lecc424 {
    UnkShotRef_lecc424 ref;
    u8 pad_0x8[0x8];
};

struct UnkShotLock_lecc424 {
    u8 pad_0x0[0x680];
    UnkWeakHandle* target;  // 0x680
};

struct UnkShotSource_lecc424 : UnkWeakTarget {
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x2b0 - 0x68];
    UnkShotLock_lecc424* lock;  // 0x2b0
};

struct UnkShotParams_lecc424 {
    s32 kind;
    u8 field_0x4;
    u8 field_0x5;
};

struct UnkShot_lecc424 {
    void** vtable;
    u8 pad_0x8[0x4];
    u32 field_0xc;
    u8 pad_0x10[0x10];
    u32 flags;  // 0x20
    u8 pad_0x24[0x4];
    UnkRefObject* field_0x28;
    UnkRefObject* field_0x30;
    u8 pad_0x38[0x30];
    UnkWeakHandle* shooter;  // 0x68
    UnkWeakHandle* source;   // 0x70
    UnkWeakHandle* target;  // 0x78
    void* field_0x80;
    f32 speed;  // 0x88
    u8 pad_0x8c[0x4];
    f32 field_0x90;
    CVector3D velocity;  // 0x94
    u8 pad_0xa0[0xc];
    CVector3D source_pos;  // 0xac
    CVector3D direction;   // 0xb8
    u8 pad_0xc4[0xc];
    CVector3D field_0xd0;
    CVector3D field_0xdc;
    CVector3D field_0xe8;
    u8 pad_0xf4[0x4];
    base::global::CRntVector<UnkShotRecord18_lecc424> field_0xf8;
    base::global::CRntVector<UnkShotRecord10_lecc424> field_0x120;
    u8 pad_0x148[0x1];
    bool field_0x149;
    u8 pad_0x14a[0x1];
    bool field_0x14b[9];
    u8 pad_0x154[0x170 - 0x154];
    f32 field_0x170;
    f32 angle;  // 0x174
    f32 spin;   // 0x178
    UnkMatrix44 rotation;  // 0x17c
    bool field_0x1bc;
    f32 field_0x1c0;
    u8 pad_0x1c4[0x28c - 0x1c4];
    bool field_0x28c;
};
static_assert(__builtin_offsetof(UnkShot_lecc424, field_0xf8) == 0xf8, "placeholder layout");
static_assert(__builtin_offsetof(UnkShot_lecc424, rotation) == 0x17c, "placeholder layout");
static_assert(__builtin_offsetof(UnkShot_lecc424, field_0x28c) == 0x28c, "placeholder layout");

static inline f32 Random(f32 lo, f32 hi) {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * (hi - lo) + lo;
}

void fn_7100ecc424(UnkShot_lecc424* self, const UnkShotParams_lecc424* params, UnkWeakTarget* shooter,
                   UnkShotSource_lecc424* source, void* arg, bool flag) {
    self->speed = g_unk_71729cf280.field_0x22b8;
    self->field_0xf8.Clear();
    self->field_0x120.Clear();

    AssignRef(&self->shooter, GetWeakHandle(shooter));
    self->field_0x80 = arg;
    if (source) {
        AssignRef(&self->source, fn_7100d78378(source));
        self->source_pos.x = source->x;
        self->source_pos.y = source->y;
        self->source_pos.z = source->z;
    } else {
        ResetRef(&self->source);
        self->source_pos.x = 0.0f;
        self->source_pos.y = 0.0f;
        self->source_pos.z = 0.0f;
    }
    self->field_0x149 = flag;

    ResetRef(&self->target);
    if (params->field_0x5) {
        self->flags |= 0x100;
        // The weak reference's assignment: the handle of what the lock's target refers to.
        UnkWeakHandle** other = &source->lock->target;
        if (&self->target != other) {
            if (UnkWeakTarget* target = *other ? (*other)->target : nullptr)
                AssignRef(&self->target, GetWeakHandle(target));
            else
                ResetRef(&self->target);
        }
    }
    if (params->kind == 2)
        self->flags |= 0x80;
    if (params->field_0x4)
        self->flags |= 0x400;
    self->field_0xc = params->field_0x4;

    self->field_0x170 = 0.0f;
    f32 speed = self->speed;
    const CVector3D direction = self->direction;
    self->velocity = CVector3D{speed * direction.x, speed * direction.y, speed * direction.z};
    f32 angle = atan2f(direction.y, direction.x);
    self->angle = angle < 0.0f ? angle + 6.28318548f : angle;
    UnkCopyMatrix(&self->rotation, fn_7100078d8c(self->angle));

    self->spin = Random(0.0f, 6.28318548f);
    self->field_0xdc = self->field_0xd0;
    self->field_0xe8 = self->field_0xd0;
    self->field_0x1c0 = 0.0f;
    self->field_0x90 = 0.0f;
    for (bool& b : self->field_0x14b)
        b = false;

    ResetRef(&self->field_0x28);
    ResetRef(&self->field_0x30);
    self->field_0x28c = false;
    self->field_0x1bc = fn_7100ed77a0(g_unk_71726af0b0) != nullptr;
}
