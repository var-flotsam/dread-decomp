// CAutclastChargeAttack (tools/tu_map.py): the attack's update. After the base update every charge entry
// (+0x90, count +0x98) is updated. In state 1 the cooldown (+0x64) runs down and, when it is spent, the
// attack picks again (fn_71003b69b0) and restarts it from the Autclast tunable (+0x28). In state 2 the
// wait (+0x68) runs down; when it is over and the owner is not frozen, a clear path from the owner along
// its up axis (fn_71003b514c) with the AI's flag (+0x121e) set ends the charge (the flag at +0x179e
// cleared, the owner's +0x220 sent its end request, state 0); otherwise the attack moves to state 3.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "game/CFrozenComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkTunable;
extern UnkTunable g_unk_71726bd2c8;  // CTunableCharClassAutclastAIComponent

struct UnkAutclastTunable_l3b6338 {
    u8 pad_0x0[0x28];
    f32 cooldown;  // 0x28
};

struct UnkEntityCollided;
UnkVec3f fn_7100d7c7a0(const UnkEntityCollided* self);

struct UnkAutclastBody_ovb0088;
bool fn_71003b514c(UnkAutclastBody_ovb0088* actor, const CVector2D* from, const CVector2D* to);

struct UnkAutclastAI_l3b6338 {
    u8 pad_0x0[0x121e];
    bool field_0x121e;
    u8 pad_0x121f[0x179e - 0x121f];
    bool field_0x179e;
};

struct UnkAutclastOwner_l3b6338 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    void* field_0x220;  // takes the attack's requests
    u8 pad_0x228[0x18];
    UnkAutclastAI_l3b6338* ai;  // 0x240
    u8 pad_0x248[0x80];
    void* frozen;  // 0x2c8
};
static_assert(__builtin_offsetof(UnkAutclastOwner_l3b6338, frozen) == 0x2c8, "layout");

struct UnkChargeEntry_l3b6338 {
    u8 raw[0x28];
};
void fn_71003b6584(UnkChargeEntry_l3b6338* entry, f32 dt);

struct UnkAutclastChargeAttack_l3b6338 {
    void** vtable;
    UnkAutclastOwner_l3b6338* owner;  // 0x8
    u8 pad_0x10[0x20];
    u32 state;  // 0x30
    u8 pad_0x34[0x30];
    f32 cooldown;  // 0x64
    f32 wait;      // 0x68
    u8 pad_0x6c[0x24];
    UnkChargeEntry_l3b6338* entries;  // 0x90
    u32 count;                        // 0x98
};
static_assert(__builtin_offsetof(UnkAutclastChargeAttack_l3b6338, entries) == 0x90, "layout");

void fn_71003b69b0(UnkAutclastChargeAttack_l3b6338* self);
void fn_71003b5fb0(UnkAutclastChargeAttack_l3b6338* self, u32 state);

inline CVector3D UpAxis(UnkAutclastOwner_l3b6338* owner) {
    const UnkMatrix44& m = owner->xform.GetMatrix();
    return CVector3D{m.m[1], m.m[5], m.m[9]};
}

void fn_71003b6338(UnkAutclastChargeAttack_l3b6338* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    for (UnkChargeEntry_l3b6338 *it = self->entries, *end = it + self->count; it != end; ++it)
        fn_71003b6584(it, dt);

    switch (self->state) {
    case 2: {
        self->wait -= dt;
        bool frozen = false;
        if (self->owner->frozen)
            frozen = fn_71007f9f20(self->owner->frozen);
        bool expired = self->wait <= 0.0f;
        if (frozen || !expired)
            return;

        UnkAutclastOwner_l3b6338* owner = self->owner;
        CVector2D from;
        UnkVec3f position = fn_7100d7c7a0(reinterpret_cast<const UnkEntityCollided*>(owner));
        fn_710007b430(&from, reinterpret_cast<const CVector3D*>(&position));
        CVector2D up;
        CVector3D axis = UpAxis(self->owner);
        fn_710007b430(&up, &axis);
        if (fn_71003b514c(reinterpret_cast<UnkAutclastBody_ovb0088*>(owner), &from, &up)) {
            UnkAutclastAI_l3b6338* ai = self->owner->ai;
            if (ai->field_0x121e) {
                // The original tests the AI for null only here, after reading it.
                if (ai)
                    ai->field_0x179e = false;
                void* target = self->owner->field_0x220;
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 0;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(target, 0x8bf36dbfdbc8ee68, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask);
                self->state = 0;
                return;
            }
        }
        fn_71003b5fb0(self, 3);
        break;
    }
    case 1:
        self->cooldown -= dt;
        if (self->cooldown < 0.0f) {
            fn_71003b69b0(self);
            self->cooldown = reinterpret_cast<UnkAutclastTunable_l3b6338*>(&g_unk_71726bd2c8)->cooldown;
        }
        break;
    }
}
