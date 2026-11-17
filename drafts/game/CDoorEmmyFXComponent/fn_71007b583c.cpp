// CDoorEmmyFXComponent unit (tools/tu_map.py). The per-frame step: unless the game is paused, tracks
// the player's distance to the door, counts down the two phase timers (+0x124, then +0x120, which
// resets every other door effect when it runs out) and plays the door's shake on its effect part.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU39.h"
#include "game/UnkTimelineComponent.h"
#include <math.h>
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkDoorEmmyFX;

extern const u64 g_unk_710164aab0;
extern const u64 g_unk_710164aab8;
extern const u64 g_unk_710164aad8;
extern const u64 g_unk_710164aac8 __attribute__((visibility("hidden")));
extern const u64 g_unk_710164aad0 __attribute__((visibility("hidden")));
extern const u64 g_unk_717271a588 __attribute__((visibility("hidden")));  // CDoorEmmyFXComponent's id
extern const u64 g_unk_717271a578 __attribute__((visibility("hidden")));
extern CType g_unk_717271a6f8 __attribute__((visibility("hidden")));      // its type

struct UnkPlayerPos_b583c {
    f32 x;
    f32 y;
    f32 z;
    UnkPlayerPos_b583c() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct UnkShakeTarget_b583c {
    u8 pad_0x0[0x48];
    void** field_0x48;
};

struct UnkShakeModel_b583c {
    u8 pad_0x0[0x78];
    UnkShakeTarget_b583c* field_0x78;
};

struct UnkShakePart_b583c {
    u8 pad_0x0[0xb];
    u8 field_0xb;  // its id in the world
    u8 pad_0xc[0x154];
    UnkShakeModel_b583c* field_0x160;
};

struct UnkDoorEmmyEntry_b583c {
    u8 pad_0x0[0x10];
    UnkReflObject* component;
};

struct UnkDoorEmmyFX_b583c {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x8];
    f32 field_0x40;  // the player's distance last frame
    f32 field_0x44;  // the range
    f32 field_0x48;  // the shake value
    f32 field_0x4c;  // the shake's time left
    f32 field_0x50;
    f32 field_0x54;
    u8 pad_0x58[0x18];
    u8 field_0x70[0xb0];
    f32 field_0x120;
    f32 field_0x124;
    f32 field_0x128;
};

bool fn_7100df5388(UnkGameManager* self, UnkPlayerPos_b583c* pos);
void fn_71007b5c48(UnkDoorEmmyFX* self_, bool on);
void fn_71001a883c(void* target, const u64* name, const f32* value);

static inline bool IsPaused(UnkGameManager* manager) {
    return reinterpret_cast<const u8*>(manager)[0x2441];
}

static inline bool IsNotNearlyZero(f32 v) {
    return !(v + 0.001f >= 0.0f) || !(v - 0.001f <= 0.0f);
}

inline f32 DoorEmmyClamp(f32 v, f32 lo, f32 hi) {
    if (v <= lo)
        return lo;
    if (v >= hi)
        return hi;
    return v;
}

inline f32 DoorEmmyLerp(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return from * (1.0f - t) + to * t;
}

// Stops every other door effect that is running or counting down.
static inline void ResetOthers() {
    UnkGameManagerIdList* list = fn_7100de94cc(g_unk_7101d47310, &g_unk_717271a588);
    void** end = list->data + list->count;
    for (void** it = list->data; it != end; ++it) {
        UnkDoorEmmyEntry_b583c* entry = static_cast<UnkDoorEmmyEntry_b583c*>(*it);
        if (!entry)
            continue;
        UnkReflObject* object = entry->component;
        if (!object)
            continue;
        const CType* type = object->vf_0x0();
        fn_71007b55e4();
        if (!fn_710009d50c(type, &g_unk_717271a6f8) || fn_710024fbc8(object))
            continue;
        UnkDoorEmmyFX_b583c* other = reinterpret_cast<UnkDoorEmmyFX_b583c*>(object);
        if (fn_710002fb70(other->field_0x70) || other->field_0x120 > 0.0f) {
            fn_71007b5c48(reinterpret_cast<UnkDoorEmmyFX*>(other), false);
            other->field_0x120 = 0.0f;
            other->field_0x124 = 0.0f;
            other->field_0x128 = 0.0f;
            fn_710002fa90(other->field_0x70);
            fn_7100d7c94c(other->owner, &g_unk_710164aac8);
            fn_7100d7c94c(other->owner, &g_unk_710164aad0);
            fn_7100d7c828(other->owner, &g_unk_710164aad8, false);
        }
    }
}

void fn_71007b583c(UnkDoorEmmyFX_b583c* self, f32 dt) {
    if (IsPaused(g_unk_7101d47310))
        return;

    UnkActor* owner = self->owner;
    f32 distance = self->field_0x44 + 1.0f;  // out of range unless the player is found
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    UnkPlayerPos_b583c pos;
    if (fn_7100df5388(g_unk_7101d47310, &pos)) {
        distance = fabsf(x - pos.x);
        f32 height = fabsf(pos.y - y - 150.0f);
        if (distance <= self->field_0x44 && height <= 300.0f && self->field_0x40 > self->field_0x44) {
            // The player just came into range: start the shake, towards the player's side.
            f32 side = x - pos.x;
            self->field_0x4c = 1.6f;
            self->field_0x54 = 0.0f;
            self->field_0x50 = static_cast<f32>((side > 0.0f) - (side < 0.0f));
        }
    }

    if (self->field_0x124 > 0.0f) {
        self->field_0x124 -= dt;
        if (self->field_0x124 <= 0.0f)
            fn_7100d7c828(self->owner, &g_unk_710164aab0, false);
    } else if (!fn_710002fb70(self->field_0x70) && self->field_0x120 > 0.0f && self->field_0x124 <= 0.0f) {
        self->field_0x120 -= dt;
        if (self->field_0x120 <= 0.0f) {
            fn_71007b5c48(reinterpret_cast<UnkDoorEmmyFX*>(self), false);
            fn_7100d7c828(self->owner, &g_unk_710164aab8, false);
            ResetOthers();
        }
    }

    if (IsNotNearlyZero(self->field_0x4c)) {
        f32 left = self->field_0x4c - dt;
        left = left > 0.0f ? left : 0.0f;
        self->field_0x4c = left;
        f32 ratio = self->field_0x4c / 1.6f;
        f32 phase = DoorEmmyLerp(self->field_0x54, self->field_0x50, ratio * ratio);
        f32 value = sinf(phase * 3.1415927f);
        if (self->field_0x4c > 0.16000001f)
            value = DoorEmmyLerp(value, self->field_0x48, DoorEmmyClamp(dt * 40.0f, 0.0f, 0.9f));
        self->field_0x48 = value;
        UnkWorld* world = static_cast<UnkWorld*>(self->owner->field_0x280);
        UnkShakePart_b583c* part = reinterpret_cast<UnkShakePart_b583c*>(fn_7101152b90(world, 0x8d01301666cc92c7));
        if (part && fn_710114e680(reinterpret_cast<UnkObject*>(world), part->field_0xb)) {
            if (void* target = *part->field_0x160->field_0x78->field_0x48)
                fn_71001a883c(target, &g_unk_717271a578, &value);
        }
    }
    self->field_0x40 = distance;
}
