// CTriggerComponent unit (tools/tu_map.py unit 1578): enabling the trigger, the counterpart of fn_7100a5f210.
// Unless already enabled (+0x202, or always with +0x200 set), runs the owner's script "OnEnable" and its
// "<scenario>.<name>" function for the name at +0x1f0; then fn_7100a5e8a4, the player's position, the timers
// of kind 3 restarted as in fn_7100a5e758, and event 4. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/global/CRntString.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CPickableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntString;

struct UnkScriptCaller;
struct UnkTriggerUpdate_ovb0067;
struct UnkTriggerComponent_ovb0067;
struct UnkTriggerActor_ovb0067;

void fn_7100a5e8a4(UnkTriggerUpdate_ovb0067* self);
void fn_7100a5ee80(UnkTriggerComponent_ovb0067* self, s32 kind, UnkTriggerActor_ovb0067* actor,
                   const UnkVec3f* dir);

// A string id's data as read here: its text at +0x10 and a use count at +0x44.
struct UnkTriggerName_la5ebf8 {
    u8 pad_0x0[0x10];
    u8 text[0x34];  // a CRntString
    u32 uses;       // 0x44
};

struct UnkTriggerScript_la5ebf8 {
    u8 pad_0x0[0x98];
    const char* name;  // 0x98
};

struct UnkTriggerOwner_la5ebf8 {
    u8 pad_0x0[0x30];
    UnkTriggerName_la5ebf8* name;  // 0x30
    u8 pad_0x38[0x1e0 - 0x38];
    UnkTriggerScript_la5ebf8* script;  // 0x1e0
};

struct UnkTriggerScenario_la5ebf8 {
    u8 pad_0x0[0x18];
    UnkTriggerName_la5ebf8* name;  // 0x18
};

struct UnkTriggerPlayer_la5ebf8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkTriggerTimer_la5ebf8 {
    u8 pad_0x0[8];
    s32 kind;  // 8
    u8 pad_0xc[0x10];
    f32 start;   // 0x1c
    f32 delay;   // 0x20
    f32 period;  // 0x24
    f32 extra;   // 0x28
};

struct UnkTrigger_la5ebf8 {
    u8 pad_0x0[0x30];
    UnkTriggerOwner_la5ebf8* owner;  // 0x30
    u8 pad_0x38[5];
    bool run_script;  // 0x3d
    u8 pad_0x3e[0xa];
    bool field_0x48;
    u8 pad_0x49[0x1f0 - 0x49];
    UnkTriggerName_la5ebf8* enable_name;  // 0x1f0: a string id
    u8 pad_0x1f8[8];
    bool always;   // 0x200
    u8 pad_0x201;
    bool enabled;  // 0x202
    u8 pad_0x203[0x240 - 0x203];
    bool has_player;                    // 0x240
    UnkVec3f player_pos;                // 0x244
    UnkTriggerTimer_la5ebf8** timers;   // 0x250
    u32 timer_count;                    // 0x258
};
static_assert(__builtin_offsetof(UnkTrigger_la5ebf8, timers) == 0x250, "layout");

// The text of a string id, counting the use.
static inline const CRntString* UseText(UnkTriggerName_la5ebf8* const& id) {
    ++id->uses;
    return reinterpret_cast<const CRntString*>(id->text);
}

static inline const CRntString* Addr(const CRntString& s) {
    return &s;
}

static inline const char* CStr(const char* s) {
    return s ? s : g_unk_710162e2ed;
}

// std::min's order: the second value when it is smaller.
static inline f32 Min(f32 a, f32 b) {
    return b < a ? b : a;
}

static inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange(f32 lo, f32 hi) {
    f32 r = Random01();
    return lo + (hi - lo) * r;
}

void fn_7100a5ebf8(UnkTrigger_la5ebf8* self) {
    self->field_0x48 = false;
    if (self->always || !self->enabled) {
        self->enabled = true;
        UnkTriggerScript_la5ebf8* script = self->owner->script;
        if (!script)
            return;
        if (self->run_script) {
            if (script->name && *script->name)
                fn_7100305088(reinterpret_cast<UnkScriptCaller*>(script), "OnEnable", g_unk_710162e2ed);
        }
        if (reinterpret_cast<base::global::UnkStringData*>(self->enable_name) != &g_unk_7101d47108->empty) {
            UnkTriggerScenario_la5ebf8* scenario =
                reinterpret_cast<UnkTriggerScenario_la5ebf8*>(g_unk_7101d47310->field_0x608);
            // "<scenario>.<name>"
            CRntString full =
                fn_710007eee0(Addr(fn_710007ef90(UseText(scenario->name), ".")), UseText(self->enable_name));
            fn_71009cde54(script, CStr(full.data), "s", CStr(UseText(self->owner->name)->data));
        }
    }
    fn_7100a5e8a4(reinterpret_cast<UnkTriggerUpdate_ovb0067*>(self));
    UnkTriggerPlayer_la5ebf8* player = reinterpret_cast<UnkTriggerPlayer_la5ebf8*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        self->has_player = true;
        self->player_pos = UnkVec3f(player->x, player->y, player->z);
    } else {
        self->has_player = false;
    }
    for (UnkTriggerTimer_la5ebf8** it = self->timers; it != self->timers + self->timer_count; ++it) {
        UnkTriggerTimer_la5ebf8* t = *it;
        if (t->kind != 3)
            continue;
        if (!(t->start >= 0.0f))
            continue;
        f32 end = t->start + t->period + t->extra;
        f32 now = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2444);
        if (!(end < now))
            continue;
        t->period = now;
        t->extra = RandomRange(0.0f, Min(t->delay, 0.0f));
    }
    fn_7100a5ee80(reinterpret_cast<UnkTriggerComponent_ovb0067*>(self), 4,
                  reinterpret_cast<UnkTriggerActor_ovb0067*>(self->owner),
                  reinterpret_cast<const UnkVec3f*>(&g_unk_7101647790));
}
