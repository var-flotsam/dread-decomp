// CShakernautAIComponent unit (tools/tu_map.py unit 1047): tries the attack the current mode picks (mode 1
// or 2) against the target, with a query carrying the horizontal distance; starts it (fn_710110592c) when its
// check passes, else tells the AI component (slot 0x220). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

extern const CClass g_unk_7172706520 __attribute__((visibility("hidden")));
extern char g_unk_7101a21540[];  // the node list's vtable

struct UnkShakernautParams_sna {
    u8 pad_0x0[0x720];
    f32 near_range;  // 0x720
    f32 threshold;  // 0x724
};

struct UnkShakernautState_sna {
    UnkShakernautParams_sna* params;  // 0x147f8
    u32 mode;  // 0x14800
    u8 pad_0xc[8];
    f32 value;  // 0x1480c
};

struct UnkShakernautActor_sna {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x210];
    void* field_0x258;
};

struct UnkShakernautAI_sna {
    u8 pad_0x0[0x30];
    UnkShakernautActor_sna* actor;  // 0x30
    u8 pad_0x38[7];
    bool field_0x3f;
    u8 pad_0x40[0x147f8 - 0x40];
    UnkShakernautState_sna state;  // 0x147f8
};

// A list of nodes (link at +8) freed through the allocator.
struct UnkTryAttackNodeList_l6b0f58 {
    void* vtable;
    u32 count;
    void* head;
    void* tail;
    base::UnkAllocator* allocator;

    ~UnkTryAttackNodeList_l6b0f58() {
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
struct UnkTryAttackRef_l6b0f58 {
    UnkRefObject* p;
    ~UnkTryAttackRef_l6b0f58() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The query an attack check takes: built by fn_71002c6f80 from the actor and the parameters.
struct UnkTryAttackQuery_l6b0f58 {
    UnkTryAttackRef_l6b0f58 ref;
    f32 distance_min;  // 0x8
    f32 distance_max;  // 0xc
    u8 pad_0x10[0x9];
    bool field_0x19;
    u8 pad_0x1a[0xe];
    UnkTryAttackNodeList_l6b0f58 nodes;  // 0x28
    u8 pad_0x50[0x8];

    UnkTryAttackQuery_l6b0f58(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkTryAttackAttack_l6b0f58 {
    void** vtable;
    u8 pad_0x8[0x60];

    bool vf_0x58(UnkTryAttackQuery_l6b0f58* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkTryAttackAttack_l6b0f58*, UnkTryAttackQuery_l6b0f58*, u32)>(
            vtable[0x58 / 8])(this, query, mode);
    }
};

struct UnkTryAttackAI_l6b0f58 {
    void** vtable;
    u8 pad_0x8[0x1138];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x24];
    u8 params[8];  // 0x1168, the attack parameters
    u8 pad_0x1170[0xae];
    u8 ready;  // 0x121e
    u8 pad_0x121f[0x146d0 - 0x121f];
    UnkTryAttackAttack_l6b0f58 attacks[2];  // 0x146d0, one for each attack mode

    const CClass* vf_0x0() {
        return reinterpret_cast<const CClass* (*)(UnkTryAttackAI_l6b0f58*)>(vtable[0])(this);
    }
    void vf_0x220(bool value) {
        reinterpret_cast<void (*)(UnkTryAttackAI_l6b0f58*, bool)>(vtable[0x220 / 8])(this, value);
    }
};
static_assert(__builtin_offsetof(UnkTryAttackAI_l6b0f58, ready) == 0x121e, "");
static_assert(__builtin_offsetof(UnkTryAttackAI_l6b0f58, attacks) == 0x146d0, "");

void fn_710110592c(UnkTryAttackAI_l6b0f58* ai, UnkTryAttackAttack_l6b0f58* attack);

bool fn_71006b0f58(UnkShakernautAI_sna* self) {
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor));
    UnkTryAttackAI_l6b0f58* ai =
        static_cast<UnkTryAttackAI_l6b0f58*>(reinterpret_cast<UnkActor*>(self->actor)->field_0x240);
    UnkTryAttackAI_l6b0f58* shakernaut = nullptr;
    if (ai) {
        const CClass* cls = ai->vf_0x0();
        fn_71006ad0a4();
        shakernaut = fn_710009d50c(cls, &g_unk_7172706520) ? ai : nullptr;
    }
    if (target && shakernaut && shakernaut->ready &&
        !fn_71011057fc(reinterpret_cast<UnkRobotSoldierAI_A2_b69*>(shakernaut))) {
        bool started = false;
        {
            UnkTryAttackQuery_l6b0f58 query(reinterpret_cast<UnkActor*>(self->actor), shakernaut->params);
            query.field_0x19 = true;
            f32 distance = __builtin_fabsf(self->actor->x - target->field_0x44);
            query.distance_min = distance;
            query.distance_max = distance;
            UnkTryAttackAttack_l6b0f58* attack = nullptr;
            if (self->state.mode == 1) {
                attack = &shakernaut->attacks[0];
                started = attack->vf_0x58(&query, shakernaut->mode);
            } else if (self->state.mode == 2) {
                attack = &shakernaut->attacks[1];
                started = attack->vf_0x58(&query, shakernaut->mode);
            }
            if (started)
                fn_710110592c(shakernaut, attack);
        }
        if (started)
            return true;
    }
    shakernaut->vf_0x220(true);
    return false;
}
