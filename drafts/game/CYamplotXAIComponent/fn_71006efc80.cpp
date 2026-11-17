// CYamplotXAIComponent's unit: fn_71006efc80 tries to start the chosen attack (the bite, kind 0, or the
// step, kind 1) on the actor's target when the AI allows attacking; when none starts, slot 0x220 is told.
// Names and layouts are placeholders.
#include "base/UnkVtableList.h"
#include "base/global/CRntVector.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a21540[];  // the node list's vtable

// A list of nodes (link at +8) freed through the allocator.
struct UnkYamplotNodeList_l6efc80 : base::UnkVtableList<void> {
    ~UnkYamplotNodeList_l6efc80() {
        vtable = g_unk_7101a21540 + 0x10;
        void* n = head;
        while (n) {
            void* next = *reinterpret_cast<void**>(static_cast<char*>(n) + 8);
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// A counted reference released on destruction.
struct UnkYamplotRef_l6efc80 {
    UnkRefObject* p;
    ~UnkYamplotRef_l6efc80() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The query an attack check takes: built by fn_71002c6f80 from the actor and the parameters.
struct UnkYamplotQuery_l6efc80 {
    UnkYamplotRef_l6efc80 ref;
    f32 field_0x8;
    f32 distance;  // 0xc
    u8 pad_0x10[0x9];
    bool field_0x19;
    u8 pad_0x1a[0xe];
    UnkYamplotNodeList_l6efc80 nodes;  // 0x28

    UnkYamplotQuery_l6efc80(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

// An attack held inside the AI component.
struct UnkYamplotAttackSlot_l6efc80 {
    u8 data[0x30];
};

struct UnkYamplotXAI_l6efc80 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x1108];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x24];
    u8 params[8];  // 0x1168, the attack parameters
    u8 pad_0x1170[0xae];
    bool can_attack;  // 0x121e
    u8 pad_0x121f[0x134a9];
    UnkYamplotAttackSlot_l6efc80 bite;  // 0x146c8
    UnkYamplotAttackSlot_l6efc80 step;  // 0x146f8
    u8 pad_0x14728[0x1a0];
    u32 attack_kind;  // 0x148c8

    void vf_0x220(bool value) {
        reinterpret_cast<void (*)(UnkYamplotXAI_l6efc80*, bool)>(vtable[0x220 / 8])(this, value);
    }
};
static_assert(__builtin_offsetof(UnkYamplotXAI_l6efc80, can_attack) == 0x121e, "layout");
static_assert(__builtin_offsetof(UnkYamplotXAI_l6efc80, bite) == 0x146c8, "layout");
static_assert(__builtin_offsetof(UnkYamplotXAI_l6efc80, attack_kind) == 0x148c8, "layout");

struct UnkBiteAttack_ybt;
bool fn_71006eca84(UnkBiteAttack_ybt* self, void* arg1, u32 arg2);
bool fn_71006ed69c(UnkAttackBase* self, void* a, u32 b);
void fn_710110592c(UnkYamplotXAI_l6efc80* self, UnkYamplotAttackSlot_l6efc80* attack);

// Starts the chosen attack on the target when its check holds on a query for the actor.
static inline bool StartAttack_l6efc80(UnkYamplotXAI_l6efc80* self, UnkActor* target) {
    UnkYamplotQuery_l6efc80 query(self->actor, self->params);
    query.field_0x19 = true;
    f32 distance = __builtin_fabsf(self->actor->field_0x44 - target->field_0x44);
    query.distance = distance;
    UnkYamplotAttackSlot_l6efc80* attack;
    switch (self->attack_kind) {
    case 1:
        query.field_0x8 = 0.0f;
        attack = &self->step;
        if (!fn_71006ed69c(reinterpret_cast<UnkAttackBase*>(attack), &query, self->mode))
            return false;
        break;
    case 0:
        query.field_0x8 = distance;
        attack = &self->bite;
        if (!fn_71006eca84(reinterpret_cast<UnkBiteAttack_ybt*>(attack), &query, self->mode))
            return false;
        break;
    default:
        return false;
    }
    fn_710110592c(self, attack);
    return true;
}

bool fn_71006efc80(UnkYamplotXAI_l6efc80* self) {
    UnkActor* target = fn_7100d7c990(self->actor);
    if (target && self->can_attack && !fn_71011057fc(reinterpret_cast<UnkRobotSoldierAI_A2_b69*>(self))) {
        if (StartAttack_l6efc80(self, target))
            return true;
    }
    self->vf_0x220(true);
    return false;
}
