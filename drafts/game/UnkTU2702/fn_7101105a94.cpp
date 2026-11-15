// Unit 2702 (hint CAIComponent): runs every attack check of the AI's list on a query built from the
// AI's attack parameters and reports whether any of them held. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a21540[];  // the node list's vtable

// A list of nodes (link at +8) freed through the allocator.
struct UnkCheckNodeList_l105a94 {
    void* vtable;
    u32 count;
    void* head;
    void* tail;
    base::UnkAllocator* allocator;

    ~UnkCheckNodeList_l105a94() {
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
struct UnkCheckRef_l105a94 {
    UnkRefObject* p;
    ~UnkCheckRef_l105a94() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkCheckQuery_l105a94 {
    UnkCheckRef_l105a94 ref;
    u8 pad_0x8[0x20];
    UnkCheckNodeList_l105a94 nodes;  // 0x28
    u8 pad_0x50[0x8];

    UnkCheckQuery_l105a94(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkCheckAttack_l105a94 {
    void** vtable;

    bool vf_0x58(UnkCheckQuery_l105a94* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkCheckAttack_l105a94*, UnkCheckQuery_l105a94*, u32)>(vtable[0x58 / 8])(
            this, query, mode);
    }
};

// The attacks, a count followed by the pointers, which start on a four-byte boundary.
struct __attribute__((packed)) UnkCheckAttackList_l105a94 {
    u32 count;
    UnkCheckAttack_l105a94* items[];
};

struct UnkCheckAI_l105a94 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1108];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x24];
    u8 params[8];  // 0x1168, the attack parameters
    u8 pad_0x1170[0x4a8];
    UnkCheckAttackList_l105a94 attacks;  // 0x1618
};

bool fn_7101105a94(UnkCheckAI_l105a94* self) {
    UnkCheckQuery_l105a94 query(self->owner, self->params);
    bool any = false;
    for (UnkCheckAttack_l105a94** it = self->attacks.items; it != self->attacks.items + self->attacks.count; ++it)
        any |= (*it)->vf_0x58(&query, self->mode);
    return any;
}
