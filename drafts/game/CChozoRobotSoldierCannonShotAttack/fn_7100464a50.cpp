// CChozoRobotSoldierCannonShotAttack's unit (tools/tu_map.py): whether the cannon shot may start. The base
// check (fn_7100360b84) must pass; a CChozoRobotSoldierAIComponent must not be cooling down, nor be held
// back (fn_7100474b20), and unless fn_7100474ae0 lets it through, no other robot soldier in the list
// (fn_710046eee4) may be busy or running this same attack. Then the shot at the owner's position
// decides (fn_7100464be8). Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAttack.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkRobotSoldierAI_b49;
bool fn_71004718fc(UnkRobotSoldierAI_b49* self);
struct UnkRobotSoldierAI_ovb0002;
bool fn_7100472fa4(UnkRobotSoldierAI_ovb0002* self);
bool fn_7100474ae0(UnkRobotSoldierAI_ovb0002* self);
bool fn_7100474b20(UnkRobotSoldierAI_ovb0002* self);
f32 fn_71004738b8(UnkRobotSoldierAI_ovb0002* self);

extern UnkTunable g_unk_71726ca688;  // the cannon shot pattern's tunables

struct UnkCannonShotTunable_l464a50 {
    u8 pad_0x0[0x30];
    f32 field_0x30;
};

struct UnkCannonShotAttacks_l464a50 {
    u8 pad_0x0[0x1a0];
    CAttack* current;  // 0x1a0
};

// The owner, its position read as words.
struct UnkCannonShotOwner_l464a50 {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0xc];
    u32 y;  // 0x54
    u8 pad_0x58[0xc];
    u32 z;  // 0x64
    u8 pad_0x68[0x240 - 0x68];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x278 - 0x248];
    UnkCannonShotAttacks_l464a50* attacks;  // 0x278
};

struct UnkCannonShotAI_l464a50 {
    u8 pad_0x0[0x30];
    UnkCannonShotOwner_l464a50* owner;  // 0x30
};

struct UnkCannonShotListEntry_l464a50 {
    u8 pad_0x0[0x10];
    UnkCannonShotAI_l464a50* ai;  // 0x10
};

struct UnkCannonShotList_l464a50 {
    UnkCannonShotListEntry_l464a50** data;
    u32 count;
};
UnkCannonShotList_l464a50* fn_710046eee4();

struct UnkCannonShotPos_l464a50 {
    u32 x, y, z;
};

struct UnkCannonShot_l464a50 {
    void** vtable;
    UnkCannonShotOwner_l464a50* owner;  // 0x8
};
bool fn_7100464be8(UnkCannonShot_l464a50* self, const UnkCannonShotPos_l464a50* pos, void* other, bool flag);

static inline UnkRobotSoldierAI_ovb0002* AsAI(void* ai) {
    return static_cast<UnkRobotSoldierAI_ovb0002*>(ai);
}

// Whether the other soldier's current attack is this one (by name).
static inline bool RunsSameAttack(UnkCannonShotAI_l464a50* other, UnkCannonShot_l464a50* self) {
    UnkCannonShotAttacks_l464a50* attacks = other->owner->attacks;
    return attacks && attacks->current &&
           *fn_7100360890(attacks->current) == *fn_7100360890(reinterpret_cast<CAttack*>(self));
}

bool fn_7100464a50(UnkCannonShot_l464a50* self, void* arg, u32 mode) {
    if (!fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode))
        return false;
    UnkReflObject* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710046ea2c())) {
        if (fn_71004718fc(reinterpret_cast<UnkRobotSoldierAI_b49*>(ai)) &&
            !(fn_71004738b8(AsAI(ai)) >= reinterpret_cast<const UnkCannonShotTunable_l464a50*>(&g_unk_71726ca688)->field_0x30))
            return false;
        bool free = !fn_7100474b20(AsAI(ai));
        if (free && !fn_7100474ae0(AsAI(ai))) {
            UnkCannonShotList_l464a50* list = fn_710046eee4();
            for (UnkCannonShotListEntry_l464a50 **it = list->data, **end = it + list->count; it != end; ++it) {
                UnkCannonShotAI_l464a50* other = *it ? (*it)->ai : nullptr;
                if (other == reinterpret_cast<UnkCannonShotAI_l464a50*>(ai))
                    continue;
                free = !fn_7100472fa4(AsAI(other)) && !fn_71004718fc(reinterpret_cast<UnkRobotSoldierAI_b49*>(other)) &&
                       !RunsSameAttack(other, self);
                if (!free)
                    break;
            }
        }
        if (!free)
            return false;
    }
    UnkCannonShotOwner_l464a50* owner = self->owner;
    UnkCannonShotPos_l464a50 pos = {owner->x, owner->y, owner->z};
    return fn_7100464be8(self, &pos, nullptr, false);
}
