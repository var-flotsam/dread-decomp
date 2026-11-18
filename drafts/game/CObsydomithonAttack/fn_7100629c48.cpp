// CObsydomithonAttack unit (tools/tu_map.py unit 945): the attack's update. After the base update it enters
// the requested stage (+0x54 into +0x50: stage 2 rearms the interrupt and its timer, stage 4 its timer,
// stage 6 tells the AI), then runs the stage: stages 2 and 3 may interrupt the attack, and when a stage's
// timer runs out the AI plays the stage's next action (stage 2 moves on to 3, stage 4 to 5). Last, it follows
// the AI's attack check (+0x147a8) and calls fn_710062eb20 when it starts passing. Names and layouts are
// placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

using base::global::CStrId;

struct UnkTunable;
extern UnkTunable g_unk_71726f8988;
extern CStrId g_unk_71726f8e08 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f8e10 __attribute__((visibility("hidden")));

struct UnkObsydomithonAttack;
f32 fn_7100629368(UnkObsydomithonAttack* self);

bool fn_7101110964(base::reflection::UnkReflObject* ai, const CStrId* action, const u64* track,
                   const base::reflection::UnkReflTypeIdArg* mode, f32 blend, f32 start);

// CCharClassObsydomithonAttack's tunable: the two stage times.
struct UnkObsydomithonTunable_l629c48 {
    u8 pad_0x0[0x2c];
    f32 aim_time;     // 0x2c
    f32 charge_time;  // 0x30
};

static inline const UnkObsydomithonTunable_l629c48& Tunable_l629c48() {
    return reinterpret_cast<const UnkObsydomithonTunable_l629c48&>(g_unk_71726f8988);
}

// The id table's slot 0x10c0 ("FullBody").
struct UnkIdTableView_l629c48 {
    u8 pad_0x0[0x10c0];
    CStrId full_body;  // 0x10c0
};

inline const CStrId& FullBodyId_l629c48() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return reinterpret_cast<const UnkIdTableView_l629c48&>(s_table).full_body;
}

struct UnkObsydomithonCheck_l629c48 {
    void** vtable;

    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkObsydomithonCheck_l629c48*)>(vtable[0x58 / 8])(this); }
};

struct UnkObsydomithonAI_l629c48 : base::reflection::UnkReflObject {
    void vf_0x668() {
        reinterpret_cast<void (*)(UnkObsydomithonAI_l629c48*)>((*reinterpret_cast<void***>(this))[0x668 / 8])(this);
    }

    u8 pad_0x8[0x179e - 0x8];
    bool interrupted;  // 0x179e
    u8 pad_0x179f[0x147a8 - 0x179f];
    UnkObsydomithonCheck_l629c48* check;  // 0x147a8
};
static_assert(__builtin_offsetof(UnkObsydomithonAI_l629c48, check) == 0x147a8, "layout");

struct UnkObsydomithonAI_l629c48;
void fn_710062eb20(UnkObsydomithonAI_l629c48* ai);

struct UnkObsydomithonOwner_l629c48 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* anim;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkObsydomithonAI_l629c48* ai;  // 0x240
};

struct UnkObsydomithonParams_l629c48 {
    u8 pad_0x0[0xf0];
    f32 limit;  // 0xf0
};

struct UnkObsydomithonAttack_l629c48 {
    void* vtable;
    UnkObsydomithonOwner_l629c48* owner;  // 0x8
    u8 pad_0x10[0x28 - 0x10];
    UnkObsydomithonParams_l629c48* params;  // 0x28
    u8 pad_0x30[0x50 - 0x30];
    s32 stage;           // 0x50
    s32 next;            // 0x54
    f32 aim_timer;       // 0x58
    f32 charge_timer;    // 0x5c
    bool interrupted;    // 0x60
    bool check_passing;  // 0x61
};

// Interrupts the attack (once per stage 2) when the value of fn_7100629368 lies strictly between -0.001 and
// the limit and the character class is not playing a CCharClassAttackFailTrack.
static inline void TryInterrupt_l629c48(UnkObsydomithonAttack_l629c48* self) {
    UnkObsydomithonOwner_l629c48* owner = self->owner;
    if (!owner)
        return;
    UnkObsydomithonAI_l629c48* ai = owner->ai;
    if (!ai)
        return;
    UnkAnimationComponent* anim = owner->anim;
    if (!anim || !self->params || self->interrupted)
        return;
    f32 value = fn_7100629368(reinterpret_cast<UnkObsydomithonAttack*>(self));
    bool failing = fn_710117b348(anim, fn_7100c0d1d8());
    if (value > -0.001f && !failing && value < self->params->limit) {
        ai->vf_0x668();
        self->interrupted = true;
        ai->interrupted = true;
    }
}

// Has the AI play `action` on the full body, in `mode` 0.
static inline bool Play_l629c48(UnkObsydomithonAI_l629c48* ai, const CStrId* action,
                                base::reflection::UnkReflTypeIdArg& mode) {
    u64 track = FullBodyId_l629c48().data->Key();
    mode.id = 0;
    return fn_7101110964(ai, action, &track, &mode, -1.0f, 0.0f);
}

void fn_7100629c48(UnkObsydomithonAttack_l629c48* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    UnkObsydomithonAI_l629c48* ai = self->owner ? self->owner->ai : nullptr;
    if (self->stage != self->next) {
        switch (self->next) {
        case 2:
            self->interrupted = false;
            if (self->params)
                self->aim_timer = Tunable_l629c48().aim_time;
            break;
        case 4:
            if (self->params)
                self->charge_timer = Tunable_l629c48().charge_time;
            break;
        case 6:
            if (ai)
                fn_710111094c(ai, 1);
            break;
        }
        self->stage = self->next;
    }
    switch (self->stage) {
    case 2:
        base::reflection::UnkReflTypeIdArg aim_mode;
        TryInterrupt_l629c48(self);
        if (self->aim_timer > 0.0f)
            self->aim_timer -= dt;
        if (ai && self->aim_timer <= 0.0f && Play_l629c48(ai, &g_unk_71726f8e08, aim_mode))
            self->next = 3;
        break;
    case 3:
        TryInterrupt_l629c48(self);
        break;
    case 4:
        base::reflection::UnkReflTypeIdArg charge_mode;
        if (self->charge_timer > 0.0f)
            self->charge_timer -= dt;
        if (ai && self->charge_timer <= 0.0f && Play_l629c48(ai, &g_unk_71726f8e10, charge_mode))
            self->next = 5;
        break;
    }

    UnkObsydomithonAI_l629c48* current = self->owner->ai;
    if (current && fn_710009d50c(current->vf_0x0(), fn_710062c51c())) {
        if (UnkObsydomithonCheck_l629c48* check = current->check) {
            bool was = self->check_passing;
            bool passing = check->vf_0x58() != 0;
            self->check_passing = passing;
            if (!was && passing)
                fn_710062eb20(current);
        }
    }
}
