// CScorpiusSpikeBallPrickAttack's unit (tools/tu_map.py unit 1022): fn_710068b9a0 starts the attack
// (fn_7100686960 with g_unk_71727037a0) when the target is more than 150 outside the Scorpius path's first
// and last points and the track allows it; fn_710068bad4 counts the attack down (+0x30): at 0 it plays the
// track's value, at -1 it picks the next count from the target's distance and the AI's mode. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkHelper;
bool fn_71009e6434(UnkHelper* helper);
struct UnkGasAttack;
void fn_7100686960(UnkGasAttack* self, const void* name, u32 arg, f32 a, f32 b);
extern __attribute__((visibility("hidden"))) char g_unk_71727037a0[];

struct UnkScorpiusPathAI_ovb0125 : UnkReflObject {
    u8 pad_0x8[0x60];
    UnkVec3f* points;  // 0x68
    u32 count;         // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
};

extern "C" int rand();
void* fn_7101177d2c(UnkAnimationComponent* self, u64 key);
struct UnkTunable;
extern UnkTunable g_unk_7172702608;

// The attack's tunables as this file reads them.
struct UnkSpikeBallTunables_ovb0125 {
    u8 pad_0x0[0x30];
    f32 field_0x30;
};

struct UnkScorpiusOwner_ovb0125 {
    u8 pad_0x0[0x208];
    UnkHelper* field_0x208;
};

struct UnkScorpiusSpikeBallPrickAttack_ovb0125 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    s32 countdown;  // 0x30
    f32 distance;   // 0x34
};

struct UnkScorpiusModeAI_ovb0125 : UnkReflObject {
    u8 pad_0x8[0x1138];
    s32 mode;  // 0x1140
};

static inline UnkScorpiusPathAI_ovb0125* ScorpiusAI(UnkActor* actor) {
    UnkReflObject* ai = static_cast<UnkReflObject*>(actor->field_0x240);
    if (!ai)
        return nullptr;
    return fn_710009d50c(ai->vf_0x0(), fn_7100690c74()) ? static_cast<UnkScorpiusPathAI_ovb0125*>(ai) : nullptr;
}

void fn_710068b9a0(UnkScorpiusSpikeBallPrickAttack_ovb0125* self, void* unused, UnkActor* target) {
    UnkHelper* helper = reinterpret_cast<UnkScorpiusOwner_ovb0125*>(fn_7100d7c990(self->owner))->field_0x208;
    if (helper && fn_71009e6434(helper))
        return;
    UnkScorpiusPathAI_ovb0125* ai = ScorpiusAI(self->owner);
    u32 last = ai->count - ((ai->closed == 0) & (ai->count != 0));
    if (last == 0)
        return;
    UnkVec3f* points = ai->points;
    f32 x = target->field_0x44;
    if (x < points[0].x - 150.0f || x > points[last].x + 150.0f) {
        auto* tracks = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
        if (tracks && fn_710117b348(tracks, fn_7100c4a358()))
            fn_7100686960(reinterpret_cast<UnkGasAttack*>(self), g_unk_71727037a0, 1, -1.0f, -1.0f);
    }
}

// Restarts the countdown by the AI's mode (+0x1140): in modes 0 and 1 it is 1 when the target is farther
// than the distance (+0x34), else a coin flip between 1 and 2; in mode 2 a coin flip between 0 and 1 when
// farther, else between 1 and 2.
static inline void Restart(UnkScorpiusSpikeBallPrickAttack_ovb0125* self) {
    f32 x = self->owner->field_0x44;
    f32 dx = __builtin_fabsf(x - fn_7100d7c990(self->owner)->field_0x44);
    UnkReflObject* object = static_cast<UnkReflObject*>(self->owner->field_0x240);
    auto* ai = fn_710009d50c(object->vf_0x0(), fn_7100690c74()) ? static_cast<UnkScorpiusModeAI_ovb0125*>(object) : nullptr;
    s32 r;
    switch (ai->mode) {
    case 0:
        if (dx > self->distance) {
            self->countdown = 1;
            return;
        }
        r = rand() & 1;
        break;
    case 1:
        if (dx > self->distance) {
            self->countdown = 1;
            return;
        }
        r = rand() & 1;
        break;
    case 2: {
        f32 limit = self->distance;
        r = rand() & 1;
        if (dx > limit) {
            self->countdown = r;
            return;
        }
        break;
    }
    default:
        return;
    }
    self->countdown = r + 1;
}

void fn_710068bad4(UnkScorpiusSpikeBallPrickAttack_ovb0125* self) {
    switch (s32 countdown = self->countdown) {
    case -1:
        Restart(self);
        break;
    case 0: {
        auto* value = static_cast<char*>(
            fn_7101177d2c(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0xf4061ff25a2163d6));
        fn_7100686960(reinterpret_cast<UnkGasAttack*>(self), value + 0x38, 1, 0.06f,
                      reinterpret_cast<UnkSpikeBallTunables_ovb0125&>(g_unk_7172702608).field_0x30);
        break;
    }
    default:
        self->countdown = countdown - 1;
        break;
    }
}
