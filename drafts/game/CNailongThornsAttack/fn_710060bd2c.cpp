// CNailongThornsAttack's unit (tools/tu_map.py): the update. By state (+0x50): 1 winds up (+0x6c against the
// description's +0xfc) and passes the progress to the owner's CNailongAIComponent; 2 waits (+0x74) and picks
// the next string id; 4 steps through the AI's thorns (+0x88, wrapping at the AI's count) at an interval that
// eases from +0x118 to +0x114 over the rounds (+0x8c of +0x120). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

using base::global::CStrId;

struct UnkThornsAttack_b68a4;
struct UnkNailongAI_b59a4;
struct UnkNailongColourAI_b67a4;

bool fn_710110c83c(void* object);
CStrId fn_710060b344(UnkThornsAttack_b68a4* self);
void fn_7100621020(UnkNailongAI_b59a4* self);
void fn_7100620ca4(UnkNailongColourAI_b67a4* self, f32 value);

struct UnkNailongAI_l60bd2c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x14699 - 0x8];
    bool field_0x14699;
    u8 pad_0x1469a[0x14850 - 0x1469a];
    s32 thorn_count;  // 0x14850
    u8 pad_0x14854[0x148c8 - 0x14854];
    u32 state;        // 0x148c8
};
static_assert(__builtin_offsetof(UnkNailongAI_l60bd2c, state) == 0x148c8, "layout");

struct UnkThornsDesc_l60bd2c {
    u8 pad_0x0[0xf8];
    f32 wait_time;    // 0xf8
    f32 windup_time;  // 0xfc
    u8 pad_0x100[0x110 - 0x100];
    u32 mode;         // 0x110
    f32 fast_interval;  // 0x114
    f32 slow_interval;  // 0x118
    u32 pad_0x11c;
    u32 rounds;       // 0x120
};

struct UnkThornsAttack_l60bd2c {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x28 - 0x10];
    UnkThornsDesc_l60bd2c* desc;  // 0x28
    u8 pad_0x30[0x50 - 0x30];
    u32 state;        // 0x50
    u8 pad_0x54[0x6c - 0x54];
    f32 windup;       // 0x6c
    bool repeat;      // 0x70
    f32 wait;         // 0x74
    CStrId id;        // 0x78
    f32 step_time;    // 0x80
    s32 last_thorn;   // 0x84
    s32 thorn;        // 0x88
    u32 round;        // 0x8c
    bool forward;     // 0x90
};
static_assert(__builtin_offsetof(UnkThornsAttack_l60bd2c, forward) == 0x90, "layout");

void fn_710060c720(UnkThornsAttack_l60bd2c* self);
void fn_710060c1bc(UnkThornsAttack_l60bd2c* self, u32 state);

// The owner's AI when it is a CNailongAIComponent.
static inline UnkNailongAI_l60bd2c* NailongAi(UnkThornsAttack_l60bd2c* self) {
    auto* ai = static_cast<UnkNailongAI_l60bd2c*>(self->owner->field_0x240);
    return ai && fn_710009d50c(ai->vf_0x0(), fn_710061f464()) ? ai : nullptr;
}

static inline void SetProgress(UnkThornsAttack_l60bd2c* self, f32 progress) {
    fn_7100620ca4(reinterpret_cast<UnkNailongColourAI_b67a4*>(NailongAi(self)), progress);
}

static inline void ResetAi(UnkThornsAttack_l60bd2c* self) {
    fn_7100621020(reinterpret_cast<UnkNailongAI_b59a4*>(NailongAi(self)));
}

// From `from` to `to` by t, held at `to` from 1 on.
static inline f32 Ease(const f32& from, const f32& to, f32 t) {
    if (t >= 1.0f)
        return to;
    return to * t + (1.0f - t) * from;
}

void fn_710060bd2c(UnkThornsAttack_l60bd2c* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    UnkNailongAI_l60bd2c* ai = NailongAi(self);
    switch (self->state) {
    case 1:
        self->windup += dt;
        if (self->windup >= self->desc->windup_time) {
            fn_710060c1bc(self, 4);
            if (ai->state != 2)
                SetProgress(self, 0.0f);
            return;
        }
        SetProgress(self, self->windup / self->desc->windup_time);
        return;
    case 2:
        self->wait += dt;
        if (self->wait < self->desc->wait_time && fn_710110c83c(ai) && !fn_7100df5740(g_unk_7101d47310)) {
            self->id = fn_710060b344(reinterpret_cast<UnkThornsAttack_b68a4*>(self));
            if (self->id.data == CStrId::Empty().data)
                return;
            if (auto* owner_ai = static_cast<UnkNailongAI_l60bd2c*>(self->owner->field_0x240))
                owner_ai->field_0x14699 = false;
            ResetAi(self);
            fn_710060c1bc(self, 3);
            return;
        }
        ResetAi(self);
        fn_710060c1bc(self, 5);
        return;
    case 4: {
        if (ai->state != 2)
            return;
        UnkThornsDesc_l60bd2c* desc = self->desc;
        if (desc->mode != 1)
            return;
        self->step_time += dt;
        f32 ratio = static_cast<f32>(self->round) / static_cast<f32>(desc->rounds);
        f32 interval = Ease(desc->slow_interval, desc->fast_interval, ratio);
        if (!(self->step_time >= interval))
            return;
        self->step_time = 0.0f;
        fn_710060c720(self);
        s32 thorn = self->thorn + (self->forward ? 1 : -1);
        self->thorn = thorn;
        if (self->forward) {
            if (thorn >= ai->thorn_count) {
                self->thorn = thorn = 0;
            }
        } else if (thorn < 0) {
            self->thorn = thorn = ai->thorn_count - 1;
        }
        if (thorn != self->last_thorn)
            return;
        if (self->round != self->desc->rounds) {
            self->round++;
            return;
        }
        fn_710060c720(self);
        self->round = 1;
        if (!self->repeat) {
            fn_710060c1bc(self, 2);
        } else {
            ResetAi(self);
            fn_710060c1bc(self, 5);
        }
        SetProgress(self, 0.0f);
        return;
    }
    }
}
