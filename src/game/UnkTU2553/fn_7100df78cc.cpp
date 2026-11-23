// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): loads the scenario `name`: keeps
// the name, clears the game manager's blackboards and loads with the scenario blackboard (and, when asked,
// a second one), after setting the pending load's string list and callback. Names and layouts are
// placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CBlackboard.h"
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
using base::global::CRntVector;

struct UnkUserDataOwner;
struct UnkStrVector;

void fn_7100e06218(void* context);

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
        allocator = GetDefaultAllocator();
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
static inline T* Singleton_ldf78cc(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// A blackboard seen through its vtable (called through a pointer, so the call stays virtual).
struct UnkBoardCall_df78cc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};
static inline void Reset_df78cc(CBlackboard& board) { reinterpret_cast<UnkBoardCall_df78cc*>(&board)->vf_0x18(); }

// The game manager's members the load uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerLoad_df78cc {
    u8 pad_0x0[0x748];
    UnkReloadCallback field_0x748;  // the pending load's callback
    u8 field_0x758[0x28];  // a string list (UnkStrVector)
    u8 pad_0x780[0x48];
    nn::os::MutexType field_0x7c8;
    u8 pad_0x7e8[0x8];
    CRntString field_0x7f0;  // the scenario's name
    CBlackboard field_0x820;
    u8 pad_0x910[0x58];
    CBlackboard field_0x968;
    u8 pad_0xa58[0x58];
    CBlackboard field_0xab0;
    u8 pad_0xba0[0x58];
    CBlackboard field_0xbf8;
    u8 pad_0xce8[0x58];
    CBlackboard field_0xd40;

    // Sets the pending load's string list and callback under the lock.
    void SetPending(u8* list, const UnkReloadCallback* callback) {
        fn_710006691c(&field_0x7c8);
        fn_71000e0524(reinterpret_cast<UnkStrVector*>(field_0x758), reinterpret_cast<UnkStrVector*>(list));
        field_0x748.context = callback->context;
        field_0x748.fn = callback->fn;
        fn_7100066920(&field_0x7c8);
    }
};
static_assert(__builtin_offsetof(UnkGameManagerLoad_df78cc, field_0x820) == 0x820, "layout");
static_assert(__builtin_offsetof(UnkGameManagerLoad_df78cc, field_0xd40) == 0xd40, "layout");

s32 fn_7100df78cc(UnkGameManager* manager, const CRntString& name, bool second_blackboard, bool flag) {
    auto* self = reinterpret_cast<UnkGameManagerLoad_df78cc*>(manager);
    fn_7101203768(Singleton_ldf78cc<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308));
    fn_7100dee46c(manager);
    CRntString_Assign(&self->field_0x7f0, &name);
    fn_7101047ea8(&self->field_0x820);
    Reset_df78cc(self->field_0x968);
    Reset_df78cc(self->field_0xab0);
    Reset_df78cc(self->field_0xbf8);
    Reset_df78cc(self->field_0xd40);
    UnkPtrList blackboards;
    blackboards.PushBack(&self->field_0x820);
    if (second_blackboard)
        blackboards.PushBack(&self->field_0xbf8);
    CRntString scenario(self->field_0x7f0);
    {
        CRntVector<CRntString> files(0xf, 1);
        files.Reserve(1);
        UnkReloadCallback callback = {manager, reinterpret_cast<void*>(&fn_7100e06218)};
        self->SetPending(reinterpret_cast<u8*>(&files), &callback);
        files.Free();
    }
    UnkUserDataOwner* owner = Singleton_ldf78cc<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308);
    CRntString none(g_unk_710162e2ed);
    return fn_71012034b0(owner, 2, &self->field_0x7f0, &scenario, &blackboards, true,
                         reinterpret_cast<void (*)()>(&fn_7100df7468), flag, &none);
}
