// Unit 2702 (a CAIComponent): picks the attack named by the current entry (+0x1610's id at +0x4e8; the
// first attack when it has none), checks it on a query built from the AI's attack parameters and resets
// (slot 0x720) when there is none or it fails. Names and layouts are placeholders.
#include "base/UnkVtableList.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

extern CStrId g_unk_7172a06360 __attribute__((visibility("hidden")));

// A list of nodes (link at +8) freed through the allocator.
struct UnkQueryNodeList_l109b84 : base::UnkVtableList<void> {
    ~UnkQueryNodeList_l109b84() {
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
struct UnkQueryRef_l109b84 {
    UnkRefObject* p;
    ~UnkQueryRef_l109b84() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The query an attack check takes: built by fn_71002c6f80 from the actor and the parameters.
struct UnkAttackQuery_l109b84 {
    UnkQueryRef_l109b84 ref;
    u8 pad_0x8[0x20];
    UnkQueryNodeList_l109b84 nodes;  // 0x28

    UnkAttackQuery_l109b84(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkAttack_l109b84 {
    void** vtable;

    bool Check(UnkAttackQuery_l109b84* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkAttack_l109b84*, UnkAttackQuery_l109b84*, u32)>(
            vtable[0x58 / 8])(this, query, mode);
    }
};

struct UnkAttackEntry_l109b84 {
    u8 pad_0x0[0x4e8];
    CStrId name;  // 0x4e8
};

struct UnkAI_l109b84 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x1108];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x24];
    u8 params[0x4a8];  // 0x1168, the attack parameters
    UnkAttackEntry_l109b84* entry;  // 0x1610
    u32 count;                      // 0x1618
    u8 attacks[0x12f5c];            // 0x161c, `count` attacks
    UnkGuiKeyList keys;             // 0x14578
    u8 pad_keys_end[0x14699 - 0x14578 - sizeof(UnkGuiKeyList)];
    bool has_attack;  // 0x14699

    UnkAttack_l109b84** Attacks() { return reinterpret_cast<UnkAttack_l109b84**>(attacks); }

    void Prepare(UnkAttackQuery_l109b84* query) {
        reinterpret_cast<void (*)(UnkAI_l109b84*, UnkAttackQuery_l109b84*)>(vtable[0x6b8 / 8])(this, query);
    }
    void Reset(bool a, f32 b) {
        reinterpret_cast<void (*)(UnkAI_l109b84*, bool, f32)>(vtable[0x720 / 8])(this, a, b);
    }
};
static_assert(__builtin_offsetof(UnkAI_l109b84, entry) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkAI_l109b84, keys) == 0x14578, "layout");
static_assert(__builtin_offsetof(UnkAI_l109b84, has_attack) == 0x14699, "layout");

// An id's string, counting the read (as CStrId::Text does).
static inline const CRntString& StringOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// Clears the selection and resets.
static inline void Stop(UnkAI_l109b84* self) {
    self->has_attack = false;
    self->Reset(true, 0.0f);
}

void fn_7101109b84(UnkAI_l109b84* self) {
    self->has_attack = false;
    UnkAttackEntry_l109b84* entry = self->entry;
    UnkAttack_l109b84* attack;
    if (entry->name.data == &g_unk_7101d47108->empty) {
        // No name: the first attack, if there is one.
        attack = self->count ? self->Attacks()[0] : nullptr;
        self->has_attack = true;
        if (!attack)
            return;
    } else {
        const CRntString& name = StringOf(entry->name);
        attack = nullptr;
        for (UnkAttack_l109b84** it = self->Attacks(); it != self->Attacks() + self->count; ++it) {
            const CStrId& id = *reinterpret_cast<const CStrId*>(fn_7100360890(reinterpret_cast<CAttack*>(*it)));
            if (StringOf(id).key == name.key) {
                attack = *it;
                break;
            }
        }
        self->has_attack = attack != nullptr;
    }
    if (attack) {
        UnkAttackQuery_l109b84 query(self->actor, self->params);
        self->Prepare(&query);
        fn_710002fd10(&self->keys, &g_unk_7172a06360);
        if (attack->Check(&query, self->mode)) {
            self->Reset(true, 0.0f);
            return;
        }
    }
    Stop(self);
}