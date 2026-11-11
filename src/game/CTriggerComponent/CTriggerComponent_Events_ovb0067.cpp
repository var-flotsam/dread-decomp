// CTriggerComponent unit (tools/tu_map.py): the component's events. fn_7100a5f554, fn_7100a5f7a8,
// fn_7100a5f884 and fn_7100a5f62c send an event (kinds 3, 1, 2 and 0) about an actor to fn_7100a5ee80
// with the unit direction from the actor's target (+0x1f0, its position at +0x84) to the actor (a zero
// vector without a target); fn_7100a5f7a8 first switches the component's sound off (fn_7100a5f3b0) and
// fn_7100a5f62c afterwards on, after handing the game manager the scenario's name followed by the
// owner's (fn_7100dfaf88) when +0x205 is set. fn_7100a5f210 removes the entries, runs the owner's
// script "OnDisable" and its "<scenario>.<name>" function, and sends event 5. Names and layouts are
// placeholders.
#include <math.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CTriggerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntString;

struct UnkScriptCaller;
bool fn_71009cde54(void* script, const char* name, const char* fmt, ...);
CRntString fn_710007ef90(const CRntString*, const char*);
extern char g_unk_7101647790[];

struct UnkTrigger_b50a5;
struct UnkTriggerEntry_b50a5;

struct UnkTriggerTarget_ovb0067 {
    u8 pad_0x0[0x84];
    f32 x, y, z;  // 0x84
};

// A string id's data as read here: its text at +0x10 and a use count at +0x44 (a union member: the
// original reloads the id after the increment).
struct UnkTriggerName_ovb0067 {
    u8 pad_0x0[0x10];
    u8 text[0x34];  // a CRntString
    union {
        u32 uses;
    };
};

struct UnkTriggerActor_ovb0067 {
    u8 pad_0x0[0x30];
    UnkTriggerName_ovb0067* name;  // 0x30
    u8 pad_0x38[0xc];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x14d];
    bool field_0x1b5;
    u8 pad_0x1b6[0x2a];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[8];
    UnkTriggerTarget_ovb0067* target;  // 0x1f0
};

struct UnkTriggerScenario_ovb0067 {
    u8 pad_0x0[0x18];
    UnkTriggerName_ovb0067* name;  // 0x18
};

struct UnkTriggerEntry_ovb0067 {
    u8 unk_0x0[0x18];
};

struct UnkTriggerComponent_ovb0067 {
    u8 pad_0x0[0x30];
    UnkTriggerActor_ovb0067* owner;  // 0x30
    u8 pad_0x38[5];
    bool field_0x3d;
    u8 pad_0x3e[0x22];
    u32 count;  // 0x60
    UnkTriggerEntry_ovb0067 entries[16];  // 0x64
    u8 pad_0x1e4[0x14];
    UnkTriggerName_ovb0067* name;  // 0x1f8: a string id
    u8 pad_0x200;
    bool field_0x201;
    u8 pad_0x202;
    bool field_0x203;
    u8 pad_0x204;
    bool field_0x205;
    u8 pad_0x206[2];
    CRntString field_0x208;
};

bool fn_7100dfbe3c(UnkGameManager*);
void fn_7100dfaf88(UnkGameManager* self, void* name, const CRntString& tag, bool flag);

// The text of a string id, counting the use.
static inline __attribute__((always_inline)) const CRntString* UseText(UnkTriggerName_ovb0067* const& id) {
    ++id->uses;
    return reinterpret_cast<const CRntString*>(id->text);
}

void fn_7100a5ee80(UnkTriggerComponent_ovb0067* self, s32 kind, UnkTriggerActor_ovb0067* actor,
                   const UnkVec3f* dir);
void fn_7100a5f3b0(UnkTriggerComponent_ovb0067* self, bool active);

static inline UnkVec3f DirFromTarget(UnkTriggerActor_ovb0067* actor) {
    UnkVec3f dir(0.0f, 0.0f, 0.0f);
    if (UnkTriggerTarget_ovb0067* t = actor->target) {
        UnkVec3f pos(actor->x, actor->y, actor->z);
        UnkVec3f d(pos.x - t->x, pos.y - t->y, pos.z - t->z);
        f32 inv = 1.0f / (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
        dir = UnkVec3f(d.x * inv, d.y * inv, d.z * inv);
    }
    return dir;
}

void fn_7100a5f554(UnkTriggerComponent_ovb0067* self, UnkTriggerActor_ovb0067* actor) {
    if (actor->field_0x1b5)
        return;
    UnkVec3f dir = DirFromTarget(actor);
    fn_7100a5ee80(self, 3, actor, &dir);
}

void fn_7100a5f7a8(UnkTriggerComponent_ovb0067* self, UnkTriggerActor_ovb0067* actor) {
    UnkVec3f dir = DirFromTarget(actor);
    fn_7100a5f3b0(self, 0);
    fn_7100a5ee80(self, 1, actor, &dir);
}

void fn_7100a5f884(UnkTriggerComponent_ovb0067* self, UnkTriggerActor_ovb0067* actor) {
    if (self->owner->field_0x1b5)
        return;
    UnkVec3f dir = DirFromTarget(actor);
    fn_7100a5ee80(self, 2, actor, &dir);
}

void fn_7100a5f62c(UnkTriggerComponent_ovb0067* self, UnkTriggerActor_ovb0067* actor) {
    if (self->owner->field_0x1b5)
        return;
    UnkVec3f dir = DirFromTarget(actor);
    fn_7100a5ee80(self, 0, actor, &dir);
    if (self->field_0x205 && fn_7100dfbe3c(g_unk_7101d47310)) {
        CRntString name(g_unk_710162e2ed);
        UnkTriggerScenario_ovb0067* scenario =
            reinterpret_cast<UnkTriggerScenario_ovb0067*>(g_unk_7101d47310->field_0x608);
        fn_710007eaa0(&name, UseText(scenario->name));
        fn_710007f3b4(&name, UseText(self->owner->name));
        fn_7100dfaf88(g_unk_7101d47310, &name, self->field_0x208, false);
    }
    fn_7100a5f3b0(self, 1);
}

static inline const CRntString* Addr(const CRntString& s) {
    return &s;
}

static inline const char* CStr(const char* s) {
    return s ? s : g_unk_710162e2ed;
}

// Removes the entries from the last one on, as fn_7100a5eb64 does.
static inline void RemoveLast(UnkTriggerComponent_ovb0067* self) {
    UnkTriggerEntry_ovb0067* e = (self->entries - 1) + self->count;
    fn_7100a5fdcc(reinterpret_cast<UnkTrigger_b50a5*>(self), reinterpret_cast<UnkTriggerEntry_b50a5**>(&e));
}

void fn_7100a5f210(UnkTriggerComponent_ovb0067* self) {
    while (self->count != 0)
        RemoveLast(self);
    if (self->field_0x201 || !self->field_0x203) {
        self->field_0x203 = true;
        UnkScriptCaller* script = self->owner->script;
        if (!script)
            return;
        if (self->field_0x3d) {
            const char* script_name = *reinterpret_cast<const char**>(reinterpret_cast<u8*>(script) + 0x98);
            if (script_name && *script_name)
                fn_7100305088(script, "OnDisable", g_unk_710162e2ed);
        }
        if (reinterpret_cast<base::global::UnkStringData*>(self->name) != &g_unk_7101d47108->empty) {
            UnkTriggerScenario_ovb0067* scenario =
                reinterpret_cast<UnkTriggerScenario_ovb0067*>(g_unk_7101d47310->field_0x608);
            // "<scenario>.<name>"
            CRntString full =
                fn_710007eee0(Addr(fn_710007ef90(UseText(scenario->name), ".")), UseText(self->name));
            fn_71009cde54(script, CStr(full.data), "s", CStr(UseText(self->owner->name)->data));
        }
    }
    fn_7100a5ee80(self, 5, self->owner, reinterpret_cast<const UnkVec3f*>(g_unk_7101647790));
}
