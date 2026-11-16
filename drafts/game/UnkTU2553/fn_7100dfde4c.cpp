// Unit 2553 (CGameManager::CTunableGameManager): marks the current scenario as known to the named object the
// id table's slot at +0x16a8 names, starts a load of kind 1 with both blackboards, and tells the script
// "slots.SetDirtyByProfileID" the scenario. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkUserDataOwner;

// A list of pointers (vtable g_unk_7101aa9ac0), its nodes from `allocator` with `flags`.
struct UnkPtrListNode {
    void* value;
    UnkPtrListNode* next;
};
struct UnkPtrList {
    void** vtable;
    // Union members: the original reloads the count, the tail and the allocator after storing a new
    // node's links.
    union {
        u32 count;
    };
    UnkPtrListNode* head;
    union {
        UnkPtrListNode* tail;
    };
    union {
        base::UnkAllocator* allocator;
    };
    union {
        u32 flags;
    };

    UnkPtrList() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkPtrList() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        for (UnkPtrListNode* n = head; n;) {
            UnkPtrListNode* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }
    void PushBack(void* value) {
        auto* node = static_cast<UnkPtrListNode*>(allocator->vf_0x40(sizeof(UnkPtrListNode), 8, flags));
        if (!node)
            return;
        node->value = value;
        node->next = nullptr;
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }
};

template <class T, u64 Size, void (*Ctor)(T*)>
static inline T* Singleton_ldfde4c(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

struct UnkMenuEntries_ovb0186;
void* fn_7101190890(UnkMenuEntries_ovb0186* self, CRntString name);

// An object registered by name, and what fn_7101190890 gives for it and a scenario.
struct UnkNamedObject_ldfde4c {
    u8 pad_0x0[0x8];
    CRntString name;
};
struct UnkNamedNode_ldfde4c {
    UnkNamedObject_ldfde4c* value;
    UnkNamedNode_ldfde4c* next;
};
struct UnkNamedUser_ldfde4c {
    u8 pad_0x0[0x88];
    bool field_0x88;
};

// The pooled data behind a string id as its text accessor sees it.
struct UnkIdTextData_ldfde4c {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    s32 reads;
};

static inline const CRntString& IdText_ldfde4c(const CStrId& id) {
    ++reinterpret_cast<UnkIdTextData_ldfde4c*>(id.data)->reads;
    return reinterpret_cast<UnkIdTextData_ldfde4c*>(id.data)->str;
}

inline UnkGameManagerIdTable& IdTable_ldfde4c() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

// The game manager's members this uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerSlots_ldfde4c {
    u8 pad_0x0[0x478];
    UnkNamedNode_ldfde4c* field_0x478;  // the objects registered by name
    u8 pad_0x480[0x370];
    CRntString field_0x7f0;  // the current scenario
    u8 field_0x820[1];       // a blackboard
    u8 pad_0x821[0x3d7];
    u8 field_0xbf8[1];  // another
};
static_assert(__builtin_offsetof(UnkGameManagerSlots_ldfde4c, field_0x7f0) == 0x7f0, "layout");
static_assert(__builtin_offsetof(UnkGameManagerSlots_ldfde4c, field_0xbf8) == 0xbf8, "layout");

s32 fn_7100dfde4c(UnkGameManager* manager) {
    auto* self = reinterpret_cast<UnkGameManagerSlots_ldfde4c*>(manager);
    fn_7101203768(Singleton_ldfde4c<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308));
    fn_7100dee46c(manager);
    UnkPtrList blackboards;
    blackboards.PushBack(self->field_0xbf8);
    blackboards.PushBack(self->field_0x820);
    const CRntString& scenario = self->field_0x7f0;
    // The slot is read as a string id here.
    const CRntString& key = IdText_ldfde4c(*reinterpret_cast<CStrId*>(IdTable_ldfde4c().pad_0x0 + 0x16a8));
    UnkNamedObject_ldfde4c* found = nullptr;
    for (UnkNamedNode_ldfde4c* n = self->field_0x478; n; n = n->next) {
        if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&n->value->name),
                          reinterpret_cast<const UnkCompared_0x30*>(&key))) {
            found = n->value;
            break;
        }
    }
    if (found) {
        auto* user = static_cast<UnkNamedUser_ldfde4c*>(
            fn_7101190890(reinterpret_cast<UnkMenuEntries_ovb0186*>(found), CRntString(scenario)));
        user->field_0x88 = true;
    }
    CRntString name(scenario);
    UnkUserDataOwner* owner = Singleton_ldfde4c<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308);
    s32 result;
    {
        CRntString none(g_unk_710162e2ed);
        result = fn_71012034b0(owner, 1, &scenario, &name, &blackboards, false,
                               reinterpret_cast<void (*)()>(&fn_7100df7468), false, &none);
    }
    const char* text = scenario.data;
    fn_7100000790("slots.SetDirtyByProfileID", "s", text ? text : g_unk_710162e2ed);
    return result;
}
