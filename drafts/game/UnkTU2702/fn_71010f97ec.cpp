// Unit 2702 (a CAIComponent): on enable, frees its pending nodes, links itself to the navigation
// mesh's two lists, registers its actor with the team and AI managers and plays "onenabled" on the
// actor's timeline. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkEmptyDtorElem.h"
#include "base/global/CName.h"
#include "base/global/CRntVector.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkActor.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTimelineCalls.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkLinkList;
struct UnkTeamList;
struct UnkWeakTarget;
struct UnkNavOwner_b0025;

void fn_7100da9e88(UnkNavOwner_b0025* self, void* item);

struct UnkNode_l0f97ec {
    u8 pad_0x0[0x20];
    UnkNode_l0f97ec* next;
};

struct UnkNodeAllocator_l0f97ec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Free(void* node);  // vf_0x48
};

struct UnkTeamFlags_l0f97ec {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
};

struct UnkAIComponent_l0f97ec {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x14f0];
    u32 count;                           // +0x1528
    UnkNode_l0f97ec* head;               // +0x1530
    UnkNode_l0f97ec* tail;               // +0x1538
    UnkNodeAllocator_l0f97ec* allocator; // +0x1540
    u8 pad_0x1548[0xc8];
    UnkTeamFlags_l0f97ec* field_0x1610;
    u8 pad_0x1618[0x1e0];
    UnkLinkList* field_0x17f8;
    u8 pad_0x1800[0x10];
    UnkLinkList* field_0x1810;
};
static_assert(__builtin_offsetof(UnkAIComponent_l0f97ec, count) == 0x1528, "layout");
static_assert(__builtin_offsetof(UnkAIComponent_l0f97ec, field_0x1610) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkAIComponent_l0f97ec, field_0x17f8) == 0x17f8, "layout");
static_assert(__builtin_offsetof(UnkAIComponent_l0f97ec, field_0x1810) == 0x1810, "layout");

static inline UnkNavMeshManager* NavMeshManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

static inline char* TeamManager() {
    if (!g_unk_71726b2398) {
        char* m = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(m);
        g_unk_71726b2398 = m;
    }
    return g_unk_71726b2398;
}

static inline UnkManager_71726af468* AIManager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

// The two lists of the navigation mesh this component registers itself in.
struct UnkNavLists_l0f97ec {
    u8 pad_0x0[0x90];
    u8 field_0x90[0x80];
    u8 field_0x110[0x8];
};

static inline void Unlink(UnkLinkList** slot) {
    if (*slot) {
        fn_71002ae87c(*slot, slot);
        *slot = nullptr;
    }
}

void fn_71010f97ec(UnkAIComponent_l0f97ec* self) {
    for (UnkNode_l0f97ec* node = self->head; node;) {
        UnkNode_l0f97ec* next = node->next;
        fn_71002874dc(node);
        self->allocator->Free(node);
        node = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;

    if (auto* lists = reinterpret_cast<UnkNavLists_l0f97ec*>(NavMeshManager()->navmesh)) {
        Unlink(&self->field_0x17f8);
        self->field_0x17f8 = reinterpret_cast<UnkLinkList*>(lists->field_0x90);
        fn_71002aeebc(self->field_0x17f8, &self->field_0x17f8);
        UnkLinkList** slot = &self->field_0x1810;
        Unlink(slot);
        *slot = reinterpret_cast<UnkLinkList*>(lists->field_0x110);
        fn_71002aeebc(*slot, slot);
    }

    if (!fn_7100deabe4(g_unk_7101d47310)) {
        UnkTeamFlags_l0f97ec* flags = self->field_0x1610;
        if (!flags || flags->field_0xc4) {
            fn_71011a726c(reinterpret_cast<UnkTeamList*>(TeamManager()),
                          reinterpret_cast<UnkWeakTarget*>(self->actor));
            fn_71011a710c(TeamManager());
        }
        fn_7100da9e88(reinterpret_cast<UnkNavOwner_b0025*>(AIManager()), self->actor);
    }

    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->actor->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        u64 key = base::global::CName("onenabled").hash;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
    }
}
