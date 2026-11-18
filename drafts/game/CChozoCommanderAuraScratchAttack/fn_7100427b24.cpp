// CChozoCommanderAuraScratchAttack's unit (tools/tu_map.py unit 645): whether the scratch may start. The
// base check (fn_710042bb38) must pass and no sentence sphere may be tracked; then, for a commander AI with
// its tunables whose readiness test (fn_7100453d84) passes and a target, the commander's value
// (fn_71010fc478) must be within 150 of the owner's x unless the AI's flag (+0x15ed0) is clear and both of
// the request's values are -1. Names and layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

// As src/game/CChozoCommanderHypersparkAttack/CChozoCommanderHypersparkAttack_Aim_ovb0023.cpp declares it.
struct UnkHypersparkAttack3_ovb0023;
bool fn_710042bb38(UnkHypersparkAttack3_ovb0023* self);

// As src/game/UnkUnit665/UnkUnit665_IsReady.cpp defines it.
struct UnkUnit665Object;
bool fn_7100453d84(const UnkUnit665Object* self);

struct UnkSphereEntry {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkSphereList {
    UnkSphereEntry** data;
    u32 count;
};

struct UnkScratchCommanderAI_l427b24 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* tunables;  // 0x10
    u8 pad_0x18[0x15ed0 - 0x18];
    bool flag;  // 0x15ed0
};
static_assert(__builtin_offsetof(UnkScratchCommanderAI_l427b24, flag) == 0x15ed0, "layout");

void fn_71010fc478(UnkScratchCommanderAI_l427b24* ai, f32* out);

struct UnkScratchRequest_l427b24 {
    u8 pad_0x0[0x8];
    f32 field_0x8;
    f32 field_0xc;
};

template <const base::reflection::CClass* (*Getter)()>
static inline UnkReflObject* Cast(UnkReflObject* p) {
    return fn_710009d50c(p->vf_0x0(), Getter()) ? p : nullptr;
}

// Whether no sentence sphere is among the tracked entries.
static inline bool NoSentenceSphere() {
    UnkSphereList* list = fn_7100ac73b0();
    bool none = true;
    if (list->count != 0) {
        UnkSphereEntry** it = list->data;
        UnkSphereEntry** end = it + list->count;
        do {
            UnkSphereEntry* entry = *it;
            UnkReflObject* object = entry ? entry->object : nullptr;
            UnkReflObject* sphere = object ? Cast<fn_710045b404>(object) : nullptr;
            if (sphere)
                none = false;
            ++it;
        } while (it != end);
    }
    return none;
}

bool fn_7100427b24(CAttack* self, UnkScratchRequest_l427b24* request) {
    if (!fn_710042bb38(reinterpret_cast<UnkHypersparkAttack3_ovb0023*>(self)) || !NoSentenceSphere())
        return false;
    UnkReflObject* object = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    if (!object || !Cast<fn_710044e708>(object))
        return true;
    auto* ai = static_cast<UnkScratchCommanderAI_l427b24*>(object);
    if (!ai->tunables || !Cast<fn_710044d0c0>(ai->tunables))
        return true;
    if (!fn_7100453d84(reinterpret_cast<const UnkUnit665Object*>(ai)))
        return false;
    bool in_reach = true;
    if (fn_7100d7c990(self->field_0x8)) {
        f32 value;
        fn_71010fc478(ai, &value);
        f32 distance = fabsf(self->field_0x8->field_0x44 - value);
        if (ai->flag || request->field_0x8 != -1.0f || request->field_0xc != -1.0f)
            in_reach = distance < 150.0f;
    }
    return in_reach;
}
