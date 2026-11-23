// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): loads the scenario `name` with no
// blackboards, after setting the pending load's string list (empty, room for one) and callback
// (fn_7100e06210 on the game manager). Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerReload.h"
#include "game/UnkPtrListNode.h"
#include "game/UnkReloadCallback.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;

struct UnkUserDataOwner;
struct UnkStrVector;

void fn_7100e06210(void* context);

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
static inline T* Singleton_ldf71d4(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

static_assert(__builtin_offsetof(UnkGameManagerReload, field_0x7f0) == 0x7f0, "layout");
static_assert(__builtin_offsetof(UnkGameManagerReload, field_0xbf8) == 0xbf8, "layout");
static_assert(__builtin_offsetof(UnkGameManagerReload, field_0x1ac0) == 0x1ac0, "layout");

s32 fn_7100df71d4(UnkGameManager* manager, const CRntString& name) {
    auto* self = reinterpret_cast<UnkGameManagerReload*>(manager);
    fn_7101203768(Singleton_ldf71d4<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308));
    fn_7100dee46c(manager);
    UnkPtrList blackboards;
    CRntString scenario(name);
    {
        CRntVector<CRntString> files(0xf);
        files.Reserve(1);
        UnkReloadCallback callback = {manager, reinterpret_cast<void*>(&fn_7100e06210)};
        self->SetPending(reinterpret_cast<u8*>(&files), &callback);
        files.Free();
    }
    UnkUserDataOwner* owner = Singleton_ldf71d4<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308);
    CRntString none(g_unk_710162e2ed);
    return fn_71012034b0(owner, 3, &name, &scenario, &blackboards, true,
                         reinterpret_cast<void (*)()>(&fn_7100df7468), false, &none);
}
