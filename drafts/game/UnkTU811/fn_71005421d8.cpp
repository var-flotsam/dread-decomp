// UnkTU811 (CEmmyAIComponent's unit): the Emmy AI's per-frame update. Steps its parts, charges a level by
// the tunable's rate (up while the direction flag is set), pulses a glow colour while it is on and pushes
// the colour to the slot table, and keeps the actor's hidden state in the timeline. Names and layouts are
// placeholders.
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CEnemyMovement.h"
#include "game/CGrabComponent.h"
#include "game/UnkActor.h"
#include "game/UnkSlots.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU811.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern "C" f32 sinf(f32);

struct UnkGfxFrameSource {
    u8 unk_0x0[0x2c];
    f32 field_0x2c;
};

struct UnkBaseComponent;
struct UnkEmmyKey_b60;
extern UnkEmmyKey_b60 g_unk_7101649de0 __attribute__((visibility("hidden")));
extern UnkEmmyKey_b60 g_unk_7101649de8 __attribute__((visibility("hidden")));
extern UnkEmmyKey_b60 g_unk_7101649df0 __attribute__((visibility("hidden")));
extern UnkEmmyKey_b60 g_unk_7101649df8 __attribute__((visibility("hidden")));

void fn_710024fb28(UnkBaseComponent*);
f32 fn_71005414c8(struct UnkEmmyAI_b0194* self);
void fn_7100542988(struct UnkEmmyAI_l5421d8* self, f32 dt);
void fn_7100543148(struct UnkEmmyAI_l5421d8* self, f32 dt);
void fn_7100543484(struct UnkEmmyAI_l5421d8* self);
void fn_7100543888(struct UnkEmmyAI_l5421d8* self, f32 dt);
void fn_71005012b8(struct UnkPart_l5421d8_a* part, f32 dt);
void fn_71004f3aa0(struct UnkPart_l5421d8_b* part, f32 dt);
void fn_71007053a0(struct UnkPart_l5421d8_c* part, f32 dt);

// A part the update steps through vtable slot 0x18.
struct UnkTick_l5421d8 {
    void** vtable;
    void Update(f32 dt) { reinterpret_cast<void (*)(UnkTick_l5421d8*, f32)>(vtable[0x18 / 8])(this, dt); }
};

struct UnkTrail_l5421d8 {
    void** vtable;
    u8 pad_0x8[0xa8];
    bool field_0xb0;
    u8 pad_0xb1[0xf];
    f32 field_0xc0;
    u8 pad_0xc4[0x3c];
    UnkWeakHandle* handle;  // 0x100
    void Update(f32 dt) { reinterpret_cast<void (*)(UnkTrail_l5421d8*, f32)>(vtable[0x18 / 8])(this, dt); }
};

struct UnkTunable_l5421d8 {
    u8 pad_0x0[0x1d0];
    f32 field_0x1d0;
    f32 field_0x1d4;
    f32 field_0x1d8;
    bool field_0x1dc;
};

struct UnkTunableSource_l5421d8 {
    void** vtable;
    UnkTunable_l5421d8* Get() {
        return reinterpret_cast<UnkTunable_l5421d8* (*)(UnkTunableSource_l5421d8*)>(vtable[0xd8 / 8])(this);
    }
};

struct UnkPart_l5421d8_a { u8 raw[0x48]; };
struct UnkPart_l5421d8_b { u8 raw[0x50]; };
struct UnkPart_l5421d8_c { u8 raw[0x10]; };

struct UnkSlotsOwner_l5421d8_c {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};
struct UnkSlotsOwner_l5421d8_b {
    u8 pad_0x0[0x50];
    UnkSlotsOwner_l5421d8_c* inner;
};

struct UnkBlockA_l5421d8 {
    UnkTick_l5421d8 tick0;
    u8 pad_0x8[0xf8];
    UnkTick_l5421d8 tick1;  // 0x100
};

struct UnkBlockB_l5421d8 {
    UnkTick_l5421d8 tick0;
    u8 pad_0x8[0x68];
    UnkTick_l5421d8 tick1;  // 0x70
    u8 pad_0x78[0x68];
    UnkTick_l5421d8 tick2;  // 0xe0
};

struct UnkEmmyAI_l5421d8 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14780 - 0x38];
    UnkBlockA_l5421d8 block_a;  // 0x14780
    u8 pad_0x14880_[0x14980 - 0x14780 - sizeof(UnkBlockA_l5421d8)];
    UnkBlockB_l5421d8 block_b;  // 0x14980
    u8 pad_0x14a68_[0x14ad0 - 0x14980 - sizeof(UnkBlockB_l5421d8)];
    UnkTrail_l5421d8 trail;  // 0x14ad0
    u8 pad_0x14bd8[0x14c0c - 0x14bd8];
    f32 speed;  // 0x14c0c
    u8 pad_0x14c10[0x27720 - 0x14c10];
    bool barrier;  // 0x27720
    u8 pad_0x27721[0x29ee0 - 0x27721];
    UnkTunableSource_l5421d8* tunable_source;  // 0x29ee0
    u8 pad_0x29ee8[0x29f14 - 0x29ee8];
    bool rising;  // 0x29f14
    u8 pad_0x29f15[3];
    f32 level;  // 0x29f18
    u8 pad_0x29f1c[0x2a010 - 0x29f1c];
    u32 slot_a;  // 0x2a010
    f32 period;  // 0x2a014
    f32 timer;  // 0x2a018
    bool hidden;  // 0x2a01c
    u8 pad_0x2a01d[3];
    base::math::CVector4D color;  // 0x2a020
    u32 slot_b;  // 0x2a030
    u8 pad_0x2a034[0x2a1d0 - 0x2a034];
    UnkPart_l5421d8_a part_a;  // 0x2a1d0
    u8 pad_0x2a218[0x2a218 - 0x2a1d0 - 0x48];

    UnkPart_l5421d8_b part_b;  // 0x2a218
    UnkPart_l5421d8_c part_c;  // 0x2a268
};

struct UnkEmmyKey_b60;

// The timeline key's mask is the all-flags default.
static inline void SetKey(void* timeline, const UnkEmmyKey_b60* key) {
    alignas(8) u32 mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7100a2cb7c(static_cast<UnkTimelineComponent*>(timeline), reinterpret_cast<const u64*>(key), &mask, false, -1.0f);
}

void fn_71005421d8(UnkEmmyAI_l5421d8* self, f32 dt) {
    fn_710024fb28(reinterpret_cast<UnkBaseComponent*>(self));
    self->block_a.tick0.Update(dt);
    self->block_a.tick1.Update(dt);
    self->block_b.tick0.Update(dt);
    self->block_b.tick1.Update(dt);
    self->block_b.tick2.Update(dt);
    self->trail.Update(dt);
    self->speed = fn_71005414c8(reinterpret_cast<UnkEmmyAI_b0194*>(self));
    f32 glow = 1.0f;
    if (self->tunable_source->Get()->field_0x1dc) {
        fn_7100542988(self, dt);
        f32 sign = self->rising ? glow : -1.0f;
        self->level += sign * (self->tunable_source->Get()->field_0x1d0 * dt);
        f32* limit;
        void* comp = self->owner->field_0x218;
        if (comp && fn_71010cc3a4(comp) == 4)
            limit = &self->tunable_source->Get()->field_0x1d4;
        else
            limit = &self->tunable_source->Get()->field_0x1d8;
        f32 level = self->level;
        if (level <= 0.0f) {
            level = 0.0f;
        } else {
            f32 max = *limit;
            level = level >= max ? max : level;
        }
        self->level = level;
        f32 total = level + self->speed;
        self->speed = !(total >= 4.0f) ? total : 4.0f;
    }
    fn_7100543148(self, dt);
    if (g_unk_7101d47328->field_0x2c - self->trail.field_0xc0 > 0.05f && self->trail.field_0xb0) {
        self->trail.field_0xb0 = false;
        const base::reflection::CType* type = self->vf_0x0();
        if (!fn_710009d50c(type, fn_71004fa488()))
            fn_7100552324(reinterpret_cast<UnkEmmyRelaxAI_b59*>(self));
    }
    fn_7100543484(self);
    self->timer = self->timer + dt;
    if (!(self->timer < self->period))
        self->timer = 0.0f;
    if (!self->barrier) {
        f32 wave = sinf(self->timer / self->period * 6.2831855f + 1.5707964f) * 0.5f + 0.5f;
        if (!(wave >= 1.0f)) {
            if (wave >= 0.0f)
                glow = wave + (1.0f - wave) * 0.02f;
            else
                glow = 0.02f;
        }
    }
    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        bool hidden = false;
        if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)) && self->barrier) {
            void* grab = self->owner->field_0x260;
            hidden = !(grab && fn_710113d83c(grab));
        }
        if (hidden != self->hidden ||
            (self->trail.handle && self->trail.handle->target && self->trail.handle->target != reinterpret_cast<UnkWeakTarget*>(target))) {
            if (target->field_0x2e8) {
                UnkWeakHandle* handle = self->trail.handle;
                bool other = handle && handle->target && handle->target != reinterpret_cast<UnkWeakTarget*>(target);
                void* grab = self->owner->field_0x260;
                if (other || (grab && fn_710113d83c(grab)))
                    SetKey(target->field_0x2e8, hidden ? &g_unk_7101649de8 : &g_unk_7101649de0);
                else
                    SetKey(target->field_0x2e8, hidden ? &g_unk_7101649df0 : &g_unk_7101649df8);
            }
            self->hidden = hidden;
        }
    }
    base::math::UnkVector4D scaled = fn_710007b5c0(&self->color, glow);
    base::math::CVector4D v;
    v.x = scaled.x <= 0.0f ? 0.0f : scaled.x;
    v.y = scaled.y <= 0.0f ? 0.0f : scaled.y;
    v.z = scaled.z <= 0.0f ? 0.0f : scaled.z;
    v.w = scaled.w <= 0.0f ? 0.0f : scaled.w;
    UnkSlots* slots = reinterpret_cast<UnkSlotsOwner_l5421d8_b*>(self->owner->field_0x228)->inner->slots;
    fn_71001c2764(slots, self->slot_a, 0, &v, 1, true);
    if (self->slot_b != 0xffffffff)
        fn_71001c2764(slots, self->slot_b, 0, &v, 1, true);
    fn_7100543888(self, dt);
    fn_71005012b8(&self->part_a, dt);
    fn_71004f3aa0(&self->part_b, dt);
    fn_71007053a0(&self->part_c, dt);
    UnkActor* target = fn_7100d7c990(self->owner);
    UnkWeakHandle* handle = self->trail.handle;
    if (handle && handle->target && handle->target != reinterpret_cast<UnkWeakTarget*>(target)) {
        if (UnkActor* old = reinterpret_cast<UnkActor*>(handle->target)) {
            if (old->field_0x2e8)
                SetKey(old->field_0x2e8, &g_unk_7101649de0);
        }
    }
    if (target)
        AssignRef(&self->trail.handle, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));
    else
        AssignRef(&self->trail.handle, static_cast<UnkWeakHandle*>(nullptr));
}
