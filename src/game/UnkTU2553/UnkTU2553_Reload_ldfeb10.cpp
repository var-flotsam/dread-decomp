// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): reloads the scenario named at +0x7f0
// with the blackboard at +0xbf8, restoring the saved string list and callback first. Names and layouts are
// placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkPtrListNode.h"
#include "game/UnkReloadCallback.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntString;

struct UnkUserDataOwner;
struct UnkStrVector;

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
static inline T* Singleton_dfeb10(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// The game manager's members the reload uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerReload {
    u8 pad_0x0[0x748];
    UnkReloadCallback field_0x748;  // the pending load's callback
    u8 field_0x758[0x28];  // a string list (UnkStrVector)
    u8 pad_0x780[0x48];
    nn::os::MutexType field_0x7c8;
    u8 pad_0x7e8[0x8];
    CRntString field_0x7f0;
    u8 pad_0x820[0x3d8];
    u8 field_0xbf8[1];  // a blackboard
    u8 pad_0xbf9[0xeb7];
    UnkReloadCallback field_0x1ab0;  // the saved callback
    u8 field_0x1ac0[0x28];  // the saved string list (UnkStrVector)

    // Sets the pending load's string list and callback under the lock.
    void SetPending(u8* list, const UnkReloadCallback* callback) {
        fn_710006691c(&field_0x7c8);
        fn_71000e0524(reinterpret_cast<UnkStrVector*>(field_0x758), reinterpret_cast<UnkStrVector*>(list));
        field_0x748.context = callback->context;
        field_0x748.fn = callback->fn;
        fn_7100066920(&field_0x7c8);
    }
};
static_assert(__builtin_offsetof(UnkGameManagerReload, field_0x7f0) == 0x7f0, "layout");
static_assert(__builtin_offsetof(UnkGameManagerReload, field_0xbf8) == 0xbf8, "layout");
static_assert(__builtin_offsetof(UnkGameManagerReload, field_0x1ac0) == 0x1ac0, "layout");

s32 fn_7100dfeb10(UnkGameManager* manager, bool flag) {
    auto* self = reinterpret_cast<UnkGameManagerReload*>(manager);
    fn_7101203768(Singleton_dfeb10<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308));
    fn_7100dee46c(manager);
    UnkPtrList blackboards;
    blackboards.PushBack(self->field_0xbf8);
    CRntString name(self->field_0x7f0);
    self->SetPending(self->field_0x1ac0, &self->field_0x1ab0);
    UnkUserDataOwner* owner = Singleton_dfeb10<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308);
    CRntString none(g_unk_710162e2ed);
    return fn_71012034b0(owner, 0, &self->field_0x7f0, &name, &blackboards, true,
                         reinterpret_cast<void (*)()>(&fn_7100df7468), flag, &none);
}
