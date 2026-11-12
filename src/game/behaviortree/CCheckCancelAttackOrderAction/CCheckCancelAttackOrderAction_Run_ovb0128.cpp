// behaviortree::CCheckCancelAttackOrderAction's unit (tools/tu_map.py, unit 466): the node's run. Unless the
// owner's +0x278 object runs something (+0x1a0), with the owner's AI component (+0x240), its attack (+0x1148)
// and fn_7100362324 holding for the attack in the AI's mode (+0x1140), the attack's check (slot 0x58) runs on
// a query built from the AI's attack parameters (+0x1168, fn_71002c6f80) with the node's flag (+0x2c); when it
// fails, the owner is passed to the global instance's fn_71011a75f0 (the instance created on first use, as
// src/game/CBehaviorTreeAIComponent/fn_7100369650.cpp does). Always returns 2. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkActor.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a21540[];  // the node list's vtable

// A list of nodes (link at +8) freed through the allocator.
struct UnkCancelNodeList_ovb0128 {
    void* vtable;
    u32 count;
    void* head;
    void* tail;
    base::UnkAllocator* allocator;

    ~UnkCancelNodeList_ovb0128() {
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

// The query an attack check takes: built by fn_71002c6f80 from the actor and the parameters.
struct UnkAttackQuery_ovb0115;

// A counted reference released on destruction.
struct UnkCancelRef_ovb0128 {
    UnkRefObject* p;
    ~UnkCancelRef_ovb0128() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkCancelQuery_ovb0128 {
    UnkCancelRef_ovb0128 ref;
    u8 pad_0x8[0x11];
    bool field_0x19;
    bool field_0x1a;
    bool field_0x1b;
    u8 pad_0x1c[0xc];
    UnkCancelNodeList_ovb0128 nodes;  // 0x28
    u8 pad_0x50[0x8];

    UnkCancelQuery_ovb0128(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkCancelAttack_ovb0128 {
    void** vtable;

    bool vf_0x58(UnkCancelQuery_ovb0128* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkCancelAttack_ovb0128*, UnkCancelQuery_ovb0128*, u32)>(
            vtable[0x58 / 8])(this, query, mode);
    }
};

struct UnkCancelAI_ovb0128 {
    u8 pad_0x0[0x1140];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x4];
    UnkCancelAttack_ovb0128* attack;  // 0x1148
    u8 pad_0x1150[0x18];
    u8 params[8];  // 0x1168, the attack parameters
};

struct UnkCancelRunner_ovb0128 {
    u8 pad_0x0[0x1a0];
    void* running;  // 0x1a0
};

struct UnkCancelOwner_ovb0128 {
    u8 pad_0x0[0x240];
    UnkCancelAI_ovb0128* ai;  // 0x240
    u8 pad_0x248[0x30];
    UnkCancelRunner_ovb0128* runner;  // 0x278
};

struct UnkCheckCancelAttackOrder_ovb0128 {
    u8 pad_0x0[0x18];
    UnkCancelOwner_ovb0128* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool flag;  // 0x2c
};

void* fn_7100362324(UnkCancelAttack_ovb0128* attack, u32 mode);

// The instance, reached through the GOT.
extern char* g_unk_71726b2398;

u32 fn_7100377664(UnkCheckCancelAttackOrder_ovb0128* self) {
    UnkCancelOwner_ovb0128* owner = self->owner;
    if (owner->runner && owner->runner->running)
        return 2;
    UnkCancelAI_ovb0128* ai = owner->ai;
    if (!ai)
        return 2;
    UnkCancelAttack_ovb0128* attack = ai->attack;
    if (!attack || !fn_7100362324(attack, ai->mode))
        return 2;
    UnkCancelQuery_ovb0128 query(reinterpret_cast<UnkActor*>(self->owner), ai->params);
    query.field_0x1a = true;
    query.field_0x19 = self->flag;
    query.field_0x1b = false;
    if (!attack->vf_0x58(&query, ai->mode)) {
        char* instance = g_unk_71726b2398;
        if (!instance) {
            instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
            fn_71011a664c(instance);
            g_unk_71726b2398 = instance;
        }
        fn_71011a75f0(instance, self->owner);
    }
    return 2;
}
