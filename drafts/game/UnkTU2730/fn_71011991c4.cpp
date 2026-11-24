// Unit 2730 (tools/tu_map.py): moves an entity into one of two lists of weak handles: it leaves the
// second list, and unless the first already has it, joins the second while the state at +0x58 is 2,
// 3 or 6 and the first otherwise (fn_71011991c4). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkReflObject;

// A counted reference to a weak handle.
struct UnkHandleRef_l1991c4 {
    UnkWeakHandle* p;

    UnkHandleRef_l1991c4() : p(nullptr) {}
    explicit UnkHandleRef_l1991c4(UnkWeakHandle* handle) : p(handle) {
        if (p)
            ++p->ref_count;
    }
    ~UnkHandleRef_l1991c4() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
    UnkHandleRef_l1991c4& operator=(const UnkHandleRef_l1991c4& other) {
        if (other.p)
            ++other.p->ref_count;
        if (p && --p->ref_count == 0)
            p->vf_0x10();
        p = other.p;
        return *this;
    }
};

// A singly linked list of weak handles (0x30 bytes), its nodes from its own allocator.
struct UnkHandleList_l1991c4 {
    struct Node {
        UnkHandleRef_l1991c4 handle;
        Node* next;
    };
    u64 field_0x0;
    u32 count;
    Node* head;
    Node* tail;
    base::IAllocator* allocator;
    u32 flags;

    // The node whose handle leads to `target`, or null.
    Node* Find(const UnkWeakTarget* target) const {
        for (Node* n = head; n; n = n->next)
            if (n->handle.p && n->handle.p->target == target)
                return n;
        return nullptr;
    }

    void PushBack(const UnkHandleRef_l1991c4& handle) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (!n)
            return;
        new (&n->handle) UnkHandleRef_l1991c4();
        n->handle = handle;
        n->next = nullptr;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            tail->next = n;
            n->next = nullptr;
            tail = n;
            ++count;
        }
    }

    // Unlinks and frees `node`; does nothing when it isn't in the list.
    void Erase(Node* node) {
        Node* prev = nullptr;
        for (Node* it = head; it;) {
            Node* next = it->next;
            if (it == node) {
                if (prev) {
                    prev->next = next;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = next;
                }
                if (--count == 0)
                    head = tail = nullptr;
                node->handle.~UnkHandleRef_l1991c4();
                allocator->vf_0x48(node);
                return;
            }
            prev = it;
            it = next;
        }
    }

    void Remove(const UnkWeakTarget* target) {
        if (Node* node = Find(target))
            Erase(node);
    }
};
static_assert(sizeof(UnkHandleList_l1991c4) == 0x30, "UnkHandleList_l1991c4 layout");

struct UnkTrackState_l1991c4 {
    u8 pad_0x0[0x4b0];
    u32 state;
};

struct UnkTracker_l1991c4 {
    u8 pad_0x0[0x58];
    UnkTrackState_l1991c4* owner;
    UnkHandleList_l1991c4 first;   // 0x60
    UnkHandleList_l1991c4 second;  // 0x90
};
static_assert(__builtin_offsetof(UnkTracker_l1991c4, second) == 0x90, "UnkTracker_l1991c4 layout");

// Whether the owner's state sends new entities to the second list.
inline bool UsesSecond_l1991c4(const UnkTrackState_l1991c4* owner) {
    switch (owner->state) {
    case 2:
    case 3:
    case 6:
        return true;
    default:
        return false;
    }
}

void fn_71011991c4(void* gui, UnkReflObject* entity) {
    UnkTracker_l1991c4* self = static_cast<UnkTracker_l1991c4*>(gui);
    UnkWeakTarget* target = reinterpret_cast<UnkWeakTarget*>(entity);
    self->second.Remove(target);
    if (self->first.Find(target))
        return;
    if (self->owner && UsesSecond_l1991c4(self->owner)) {
        if (self->second.Find(target))
            return;
        UnkHandleRef_l1991c4 handle(fn_7100d78378(target));
        self->second.PushBack(handle);
    } else {
        UnkHandleRef_l1991c4 handle(fn_7100d78378(target));
        self->first.PushBack(handle);
    }
}
