// behaviortree::CCanDoAttackCondition's unit (tools/tu_map.py): the condition's check (the attack query as src/game/behaviortree/CCheckCancelAttackOrderAction/CCheckCancelAttackOrderAction_Run_ovb0128.cpp builds it).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a21540[];  // the node list's vtable

// A list of nodes (link at +8) freed through the allocator.
struct UnkCanAttackNodeList_ovb0137 {
    void* vtable;
    u32 count;
    void* head;
    void* tail;
    base::UnkAllocator* allocator;

    ~UnkCanAttackNodeList_ovb0137() {
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

struct UnkAttackQuery_ovb0115;
void fn_71002c6f80(UnkAttackQuery_ovb0115* self, UnkActor* actor, void* params);
struct UnkScorpiusAI_b69a4;

// A counted reference released on destruction.
struct UnkCanAttackRef_ovb0137 {
    UnkRefObject* p;
    ~UnkCanAttackRef_ovb0137() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The query an attack check takes (fn_71002c6f80 builds it from the actor and the parameters).
struct UnkCanAttackQuery_ovb0137 {
    UnkCanAttackRef_ovb0137 ref;
    u8 pad_0x8[0x20];
    UnkCanAttackNodeList_ovb0137 nodes;  // 0x28
    u8 pad_0x50[0x8];

    UnkCanAttackQuery_ovb0137(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkCanAttackAttack_ovb0137 {
    void** vtable;

    bool vf_0x58(UnkCanAttackQuery_ovb0137* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkCanAttackAttack_ovb0137*, UnkCanAttackQuery_ovb0137*, u32)>(
            vtable[0x58 / 8])(this, query, mode);
    }
};

struct UnkCanAttackAI_ovb0137 {
    u8 pad_0x0[0x1140];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x4];
    UnkCanAttackAttack_ovb0137* attack;  // 0x1148
    u8 pad_0x1150[0x18];
    u8 params[8];  // 0x1168, the attack parameters
};

struct UnkCanAttackActor_ovb0137 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkCanAttackAI_ovb0137* ai;  // 0x240
};

// The condition's attack name (a string: its text first).
struct UnkCanAttackName_ovb0137 {
    const char* data;
};

struct UnkCanDoAttack_ovb0137 {
    u8 pad_0x0[0x18];
    UnkCanAttackActor_ovb0137* actor;  // 0x18
    u8 pad_0x20[0x18];
    UnkCanAttackName_ovb0137 name;  // 0x38
};

// Whether the attack (the one named at +0x38, or the AI's current one) can be done: its check (slot
// 0x58) holds on a query for the actor in the AI's mode, and the actor is not turning (or is aborting
// a turn).
bool fn_710039865c(UnkCanDoAttack_ovb0137* self) {
    UnkActor* target = self->actor ? fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)) : nullptr;
    UnkCanAttackAI_ovb0137* ai = self->actor ? self->actor->ai : nullptr;
    UnkAnimationComponent* animation = self->actor ? self->actor->animation : nullptr;
    UnkCanAttackAttack_ovb0137* attack;
    if (self->name.data && self->name.data[0]) {
        if (!ai)
            return false;
        attack = static_cast<UnkCanAttackAttack_ovb0137*>(
            fn_7101109df4(reinterpret_cast<UnkScorpiusAI_b69a4*>(ai), &self->name));
    } else {
        if (!ai)
            return false;
        attack = ai->attack;
    }
    if (!target || !attack)
        return false;
    bool ok;
    {
        UnkCanAttackQuery_ovb0137 query(reinterpret_cast<UnkActor*>(self->actor), ai->params);
        ok = attack->vf_0x58(&query, ai->mode);
    }
    if (!ok)
        return false;
    if (!fn_710117b348(animation, fn_7100d2b3a8()))
        return true;
    return fn_710117b348(self->actor->animation, fn_7100bfac38());
}
