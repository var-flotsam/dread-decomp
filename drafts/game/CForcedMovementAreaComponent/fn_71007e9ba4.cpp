// CForcedMovementAreaComponent's unit (1256): finds the record kept for `who`'s id (+0x30) and, in it, the
// reference whose target has `what`'s id (+0x20); unregisters the component's listener (+0x38) from that
// target's list (+0x128) and drops the reference. A record left empty ends (fn_71007ec5e4) and is erased
// (fn_71007ec684). Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkForcedMovementArea_A5_b68;
struct UnkForcedMovementArg_A5_b68;
void fn_71007ec5e4(UnkForcedMovementArea_A5_b68* self, UnkForcedMovementArg_A5_b68* arg);
void* CForcedMovementAreaComponent_EraseIdRefVecEntry(char* array, void* it);

struct UnkAreaListenersTag_l7e9ba4;
using UnkAreaListeners_l7e9ba4 = base::UnkSList<UnkAreaListenersTag_l7e9ba4>;
using UnkAreaListenerNode_l7e9ba4 = UnkAreaListeners_l7e9ba4::Node;

static inline s32 IndexOf(UnkAreaListeners_l7e9ba4* list, u64 value) {
    u32 i = 0;
    for (UnkAreaListenerNode_l7e9ba4* n = list->head; n; n = n->next, ++i) {
        if (n->value == value)
            return static_cast<s32>(i);
    }
    return -1;
}

static inline UnkAreaListenerNode_l7e9ba4* Find(UnkAreaListeners_l7e9ba4* list, u64 value) {
    for (UnkAreaListenerNode_l7e9ba4* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkAreaListeners_l7e9ba4* list, UnkAreaListenerNode_l7e9ba4* node) {
    UnkAreaListenerNode_l7e9ba4* prev = nullptr;
    UnkAreaListenerNode_l7e9ba4* it = list->head;
    while (it) {
        UnkAreaListenerNode_l7e9ba4* cur = it;
        UnkAreaListenerNode_l7e9ba4* next = it->next;
        it = next;
        if (cur != node) {
            prev = cur;
            continue;
        }
        if (prev) {
            prev->next = next;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

struct UnkAreaTarget_l7e9ba4 {
    u8 pad_0x0[0x20];
    u64 id;  // 0x20
    u8 pad_0x28[0x100];
    UnkAreaListeners_l7e9ba4 listeners;  // 0x128
};

struct UnkAreaRefObject_l7e9ba4 : UnkRefObject {
    UnkAreaTarget_l7e9ba4* target;  // 0x10
};

// A counted reference to a target. The pointer is a union member: the original reloads it after the
// count increment.
struct UnkAreaRef_l7e9ba4 {
    union {
        UnkAreaRefObject_l7e9ba4* ptr;
    };

    UnkAreaRef_l7e9ba4(const UnkAreaRef_l7e9ba4& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkAreaRef_l7e9ba4() { Release(ptr); }

    static void Release(UnkAreaRefObject_l7e9ba4* p) {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
    void Set(UnkAreaRefObject_l7e9ba4* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            Release(ptr);
            ptr = p;
        }
    }
    UnkAreaTarget_l7e9ba4* Target() const { return ptr ? ptr->target : nullptr; }
};

using UnkAreaRefVec_l7e9ba4 = base::global::CRntVector<UnkAreaRef_l7e9ba4>;

// Records with a string id alongside each.
struct UnkAreaRecords_l7e9ba4 {
    UnkAreaRefVec_l7e9ba4* records;
    base::global::CStrId* ids;
    u32 count;

    UnkAreaRefVec_l7e9ba4* Find(const base::global::CStrId& id) {
        u32 n = count;
        for (u32 i = 0; i < n; ++i) {
            if (ids[i] == id)
                return &records[i];
        }
        return &records[n];
    }
    UnkAreaRefVec_l7e9ba4* End() { return &records[count]; }
};

// The reference whose target has `id`, or the end.
static UnkAreaRef_l7e9ba4* FindRef(UnkAreaRefVec_l7e9ba4* refs, const u64& id) {
    u32 n = refs->count;
    for (u32 i = 0; i < n; ++i) {
        if (id == refs->data[i].ptr->target->id)
            return &refs->data[i];
    }
    return &refs->data[n];
}

// Destroys the reference at `index` and moves the later ones down.
static void EraseAt(UnkAreaRefVec_l7e9ba4* refs, u32 index) {
    if (index < refs->count) {
        refs->data[index].~UnkAreaRef_l7e9ba4();
        for (u32 i = index; i < refs->count - 1; ++i) {
            new (&refs->data[i]) UnkAreaRef_l7e9ba4(refs->data[i + 1]);
            refs->data[i + 1].~UnkAreaRef_l7e9ba4();
        }
        refs->count = refs->count - 1;
    }
}

struct UnkForcedMovementArea_l7e9ba4 {
    u8 pad_0x0[0x38];
    u8 listener[0x10];               // 0x38
    UnkAreaRecords_l7e9ba4 records;  // 0x48
};

struct UnkAreaWhat_l7e9ba4 {
    u8 pad_0x0[0x20];
    u64 id;  // 0x20
};

struct UnkAreaWho_l7e9ba4 {
    u8 pad_0x0[0x30];
    base::global::CStrId id;  // 0x30
};

void fn_71007e9ba4(UnkForcedMovementArea_l7e9ba4* self, UnkAreaWhat_l7e9ba4* what, UnkAreaWho_l7e9ba4* who) {
    UnkAreaRefVec_l7e9ba4* refs = self->records.Find(who->id);
    if (refs == self->records.End())
        return;

    UnkAreaRef_l7e9ba4* it = FindRef(refs, what->id);

    UnkAreaListeners_l7e9ba4* list = &it->Target()->listeners;
    u64 key = reinterpret_cast<u64>(self->listener);
    if (IndexOf(list, key) >= 0) {
        if (UnkAreaListenerNode_l7e9ba4* node = Find(list, key))
            Erase(list, node);
    }

    EraseAt(refs, it - refs->data);
    if (refs->count == 0) {
        fn_71007ec5e4(reinterpret_cast<UnkForcedMovementArea_A5_b68*>(self),
                      reinterpret_cast<UnkForcedMovementArg_A5_b68*>(who));
        CForcedMovementAreaComponent_EraseIdRefVecEntry(reinterpret_cast<char*>(&self->records), refs);
    }
}
