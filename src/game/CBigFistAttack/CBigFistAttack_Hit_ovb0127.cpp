// CBigFistAttack's unit (tools/tu_map.py): whether the attack may start (as CGoliathAttack's
// fn_710058e488 in src/game/CGoliathAttack/CGoliathAttack_Shot_ovb0116.cpp, with the BigFist AI for the
// distance test) and its hit message handler, which reacts to hits on objects with certain names.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CBaseBigFistAIComponent.h"
#include "game/CCharClassBigFistAttack.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkBigFistAI_gla;
// The BigFist tunable (registered in src/reflection/CCharClassBigFistAttack.cpp); its +0x30 is a
// distance.
struct UnkTunable;

static inline f32 TunableDistance() {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_71726c06a8) + 0x30);
}

// The actor's AI component (+0x240) as the class `getter` gives, or null.
template <const base::reflection::CClass* (*Getter)()>
static inline UnkBigFistAI_gla* CastAI(UnkActor* actor) {
    UnkReflObject* ai = static_cast<UnkReflObject*>(actor->field_0x240);
    if (!ai)
        return nullptr;
    return fn_710009d50c(ai->vf_0x0(), Getter()) ? reinterpret_cast<UnkBigFistAI_gla*>(ai) : nullptr;
}

bool fn_71003e0908(CAttack* self, void* arg, u32 mode) {
    bool ok = fn_7100360b84(self, arg, mode);
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    bool result = false;
    if (ok && target) {
        UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
        if (sub && sub->field_0x820) {
            f32 distance = TunableDistance();
            result = !fn_71003e6c04(CastAI<fn_71003e70dc>(self->field_0x8), distance);
        }
    }
    UnkBigFistAI_gla* ai = CastAI<fn_71003e53c4>(self->field_0x8);
    if (result && ai) {
        s32 out = 0;
        result = fn_71003e60ac(ai, &out);
    }
    return result;
}


// A string id's entry as these checks see it (base::global::UnkStringEntry): the text at +0x10 and
// a use count at +0x44, a union member because its store makes the original reload the id.
struct UnkBigFistNameEntry_ovb0127 {
    u8 pad_0x0[0x10];
    base::global::CRntString text;  // 0x10
    u8 pad_0x40[0x4];
    union {
        u32 uses;  // 0x44
    };
};

// Ids whose text a hit's name may contain, and ids it may be.
extern UnkBigFistNameEntry_ovb0127* g_unk_71726c0538 __attribute__((visibility("hidden")));
extern UnkBigFistNameEntry_ovb0127* g_unk_71726c0540 __attribute__((visibility("hidden")));
extern UnkBigFistNameEntry_ovb0127* g_unk_71726c0548 __attribute__((visibility("hidden")));
extern UnkBigFistNameEntry_ovb0127* g_unk_71726c0550[3] __attribute__((visibility("hidden")));

static inline const char* Text(UnkBigFistNameEntry_ovb0127* const& id) {
    ++id->uses;
    const char* s = id->text.data;
    return s ? s : g_unk_710162e2ed;
}

static inline bool Contains(UnkBigFistNameEntry_ovb0127* const& name, UnkBigFistNameEntry_ovb0127* const& part) {
    ++name->uses;
    if (name == reinterpret_cast<UnkBigFistNameEntry_ovb0127*>(&g_unk_7101d47108->empty))
        return false;
    return fn_710007fb9c(&name->text, Text(part));
}

struct UnkBigFistHitTarget_ovb0127 {
    u8 pad_0x0[0x38];
    UnkBigFistNameEntry_ovb0127* name;  // 0x38
};

struct UnkBigFistHitRef_ovb0127 {
    u8 pad_0x0[0x10];
    UnkBigFistHitTarget_ovb0127* target;  // 0x10
};

struct UnkBigFistMessage_ovb0127 {
    s32 type;
    u8 pad_0x4[0x4];
    UnkBigFistHitRef_ovb0127* hit;  // 0x8
};

struct UnkBigFistAttackState_ovb0127 {
    u8 pad_0x0[0x30];
    s32 state;  // 0x30
};

void fn_71003e1134(UnkBigFistAttackState_ovb0127* self);

bool fn_71003e0fb4(UnkBigFistAttackState_ovb0127* self, UnkBigFistMessage_ovb0127* msg) {
    if (msg->type == 1 && self->state == 1) {
        UnkBigFistHitTarget_ovb0127* target = msg->hit->target;
        if (Contains(target->name, g_unk_71726c0538) || Contains(target->name, g_unk_71726c0540) ||
            Contains(target->name, g_unk_71726c0548))
            return true;
        UnkBigFistNameEntry_ovb0127* name = target->name;
        if (name == g_unk_71726c0550[0] || name == g_unk_71726c0550[1] || name == g_unk_71726c0550[2])
            fn_71003e1134(self);
    }
    return true;
}
