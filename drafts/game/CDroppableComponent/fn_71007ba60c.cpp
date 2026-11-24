// CDroppableComponent unit (tools/tu_map.py unit 1216): its update. Ages the drop, destroys it when it has
// been resting too long, and while the player is around either collects it (close enough) or pulls it
// toward the player with a speed that grows as it gets closer, or lets it drift and slow down; the drop's
// loop sound follows its speed. Names and layouts are placeholders.
#include <math.h>

#include "base/snd/CSoundSystem.h"
#include "game/CActorComponent.h"
#include "game/CComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkSoundRef.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkDropHolder_l7ba60c {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkDropPlayerItem_l7ba60c {
    u8 pad_0x0[0x170];
    f32 field_0x170;
    f32 field_0x174;
    u8 pad_0x178[4];
    f32 field_0x17c;
    bool field_0x180;
    bool field_0x181;
    u8 pad_0x182[6];
    bool field_0x188;
};

struct UnkDropTunables_l7ba60c {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
};

struct UnkDropPlayerPos_l7ba60c {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkDroppable_l7ba60c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[8];
    f32 limit;  // 0x40
    f32 age;    // 0x44
    f32 resting;  // 0x48
    bool field_0x4c;
    u8 pad_0x4d[3];
    f32 speed;  // 0x50
    f32 dir[3];  // 0x54
    bool fleeing;  // 0x60
    u8 pad_0x61[3];
    f32 fleeDir[3];  // 0x64
    f32 fleeSpeed;  // 0x70
    u8 pad_0x74[4];
    UnkSoundHandle sound;  // 0x78
    u8 pad_0x88[0x18];
    UnkDropHolder_l7ba60c* holder;  // 0xa0

    bool vf_0x178(const UnkVec3f* from, const UnkVec3f* to) {
        return reinterpret_cast<bool (*)(UnkDroppable_l7ba60c*, const UnkVec3f*, const UnkVec3f*)>(vtable[0x178 / 8])(
            this, from, to);
    }
};

UnkDropPlayerItem_l7ba60c* fn_71007bad34(UnkPlayer* player);
void fn_71009ce294(void* target, UnkPlayer* player);
extern UnkDropTunables_l7ba60c g_unk_717271ab10;

static inline UnkSound* GetSound_l7ba60c(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

void fn_71007ba60c(UnkDroppable_l7ba60c* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    self->age += dt;
    bool blocked = fn_7100d7bea0(self->owner, 0);
    if (self->limit > 0.0f && !(blocked | !(self->age > self->limit)))
        self->resting += dt;
    if (blocked) {
        self->resting = 0.0f;
    } else if (self->resting > 3.0f) {
        fn_7100232c68(self->owner);
        return;
    }

    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    if (!fn_710024fbc8(self->holder ? self->holder->target : nullptr))
        return;

    UnkDropPlayerItem_l7ba60c* item = fn_71007bad34(player);
    UnkActor* owner = self->owner;
    UnkVec3f pos0(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    UnkDropPlayerPos_l7ba60c* target = reinterpret_cast<UnkDropPlayerPos_l7ba60c*>(player);
    f32 dx = target->x - pos0.x;
    f32 dy = target->y - pos0.y;
    f32 dz = target->z - pos0.z;
    f32 d2 = dx * dx + dy * dy + dz * dz;
    bool flag = item->field_0x180;
    bool close = d2 < 40000.0f && !flag;

    bool chase = item ? (flag || d2 < 40000.0f) : close;
    if (chase) {
        f32 dist = sqrtf(d2);
        if (d2 > 0.0f && d2 >= 2500.0f) {
            f32 e = d2 >= 1690000.0f ? 1690000.0f : d2;
            f32 inv = 1.0f / (dist + 0.0001f);
            f32 nx = dx * inv, ny = dy * inv, nz = dz * inv;
            f32 pull = ((1690000.0f + 1000.0f - e) / 1690000.0f) * 1700.0f;
            f32 grow = self->age > 0.0f ? (self->age < 1.0f ? self->age : 1.0f) : 0.0f;
            pull = pull * grow;
            f32 speed = 0.0f;
            if (close) {
                self->speed = self->speed / 0.85f;
                f32 s = self->speed + 100.0f;
                if (s > 0.0f)
                    speed = s >= pull ? pull : s;
            } else if (!item->field_0x181 && !item->field_0x188) {
                f32 span = item->field_0x174 - item->field_0x170;
                f32 t = 0.0f;
                if (span > 0.0f) {
                    f32 r = (item->field_0x17c - item->field_0x170) / span;
                    if (r > 0.0f)
                        t = r < 1.0f ? r : 1.0f;
                }
                speed = pull * t;
            }
            UnkVec3f next(nx * speed * dt + pos0.x, ny * speed * dt + pos0.y, nz * speed * dt + pos0.z);
            self->owner->vf_0xd0(&next, true);
            if (self->vf_0x178(&pos0, &next))
                self->field_0x4c = !self->field_0x4c;
            if (!GetSound_l7ba60c(&self->sound))
                self->sound = fn_7100d7c45c(reinterpret_cast<UnkEntityWithSound*>(self->owner), nullptr, nullptr, 0);
            if (UnkSound* sound = GetSound_l7ba60c(&self->sound)) {
                fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(sound), self->speed / 1700.0f, false, 0.2f);
                UnkSound* again = GetSound_l7ba60c(&self->sound);
                f32 v = self->speed / 1700.0f + 1.0f;
                fn_7100105da8(reinterpret_cast<UnkInterp_7100105d34*>(again), v > 0.0f ? (v < 2.0f ? v : 2.0f) : 0.0f,
                              0.2f);
            }
            self->speed = speed;
            self->dir[0] = nx;
            self->dir[1] = ny;
            self->dir[2] = nz;
            return;
        }
        // Close enough: the drop is collected.
        if (self->holder && self->holder->target)
            fn_71009ce294(self->holder->target, player);
        if (GetSound_l7ba60c(&self->sound)) {
            fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(self->owner), &self->sound, 0.0f, 0.0f);
            self->sound = g_unk_7108b6c460;
        }
        fn_7100232c68(self->owner);
        return;
    }

    if (self->fleeing) {
        f32 step = self->fleeSpeed * g_unk_717271ab10.field_0x24;
        step = step > 0.0f ? (step >= g_unk_717271ab10.field_0x28 ? g_unk_717271ab10.field_0x28 : step) : 0.0f;
        step = step * dt;
        self->fleeSpeed = self->fleeSpeed - step;
        UnkVec3f next(step * self->fleeDir[0] + pos0.x, step * self->fleeDir[1] + pos0.y,
                      step * self->fleeDir[2] + pos0.z);
        self->owner->vf_0xd0(&next, true);
        if (!(self->fleeSpeed < 5.0f))
            return;
        self->fleeing = false;
        return;
    }
    if (self->speed > 0.0f) {
        if (!self->field_0x4c)
            self->speed = self->speed * 0.85f;
        f32 speed = self->speed;
        UnkVec3f next(speed * self->dir[0] * dt + pos0.x, speed * self->dir[1] * dt + pos0.y,
                      speed * self->dir[2] * dt + pos0.z);
        self->owner->vf_0xd0(&next, true);
        if (self->vf_0x178(&pos0, &next))
            self->field_0x4c = !self->field_0x4c;
    } else if (GetSound_l7ba60c(&self->sound)) {
        fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(self->owner), &self->sound, 0.0f, 0.0f);
        self->sound = g_unk_7108b6c460;
    }
}
