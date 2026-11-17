// Unit 2578 (no reflected class): the sound-id manager's pause (fn_7100f022c4, called by the controller applet). Pausing
// pauses each playing instance (fn_710021f950) not paused already or in state 7 and keeps it in the paused list at
// +0xe0; resuming resumes each paused one (fn_710021fa20) and empties the list. Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "game/UnkTU324.h"
#include "types.h"

struct UnkSoundIdInstance {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    u8 pad_0xc[0x20];
    u32 state;  // 0x2c
};

struct UnkSoundIdRef {
    UnkSoundIdInstance* ptr;

    UnkSoundIdRef(const UnkSoundIdRef& other) : ptr(other.ptr) {
        if (ptr)
            __atomic_fetch_add(&ptr->ref_count, 1, __ATOMIC_SEQ_CST);
    }
    ~UnkSoundIdRef() {
        if (UnkSoundIdInstance* p = ptr) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__builtin_expect(__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0, 1))
                p->vf_0x10();
        }
    }
};

struct UnkSoundIdListNode {
    u32 key;
    UnkSoundIdRef ref;
    UnkSoundIdListNode* next;
};

u32 fn_710021fa20(UnkMedia324* self);

static inline void AddRef_lf022c4(UnkSoundIdInstance* p) {
    __atomic_fetch_add(&p->ref_count, 1, __ATOMIC_SEQ_CST);
}

static inline void Release_lf022c4(UnkSoundIdInstance* p) {
    __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
    if (__builtin_expect(__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0, 1))
        p->vf_0x10();
}

// A counted reference to an instance that can be reassigned.
struct UnkSoundIdHandle_lf022c4 {
    UnkSoundIdInstance* ptr;

    UnkSoundIdHandle_lf022c4() : ptr(nullptr) {}
    UnkSoundIdHandle_lf022c4(const UnkSoundIdHandle_lf022c4& other) : ptr(other.ptr) {
        if (ptr)
            AddRef_lf022c4(ptr);
    }
    UnkSoundIdHandle_lf022c4& operator=(UnkSoundIdInstance* p) {
        if (p)
            AddRef_lf022c4(p);
        if (UnkSoundIdInstance* old = ptr)
            Release_lf022c4(old);
        ptr = p;
        return *this;
    }
    ~UnkSoundIdHandle_lf022c4() {
        if (ptr)
            Release_lf022c4(ptr);
    }
};

struct UnkPausedNode_lf022c4 {
    UnkSoundIdHandle_lf022c4 ref;
    UnkPausedNode_lf022c4* next;

    explicit UnkPausedNode_lf022c4(UnkSoundIdInstance* instance) {
        ref = instance;
        next = nullptr;
    }
};

struct UnkPausedList_lf022c4 : base::UnkLinkedList<UnkPausedNode_lf022c4> {
    // The position of `instance` in the list, or -1.
    s32 IndexOf(const UnkSoundIdInstance* instance) const {
        u32 i = 0;
        for (UnkPausedNode_lf022c4* n = head; n; n = n->next, ++i)
            if (n->ref.ptr == instance)
                return (s32)i;
        return -1;
    }

    void Add(UnkSoundIdInstance* instance) {
        void* mem = allocator->vf_0x40(sizeof(UnkPausedNode_lf022c4), 8, flags);
        if (mem)
            PushBack(new (mem) UnkPausedNode_lf022c4(instance));
    }
};

struct UnkStruct_0x128 {
    u8 pad_0x0[0x80];
    base::UnkLinkedList<UnkSoundIdListNode> field_0x80;
    u8 pad_0xa8[0x38];
    UnkPausedList_lf022c4 paused;  // 0xe0
};
static_assert(__builtin_offsetof(UnkStruct_0x128, paused) == 0xe0, "");

void fn_7100f022c4(UnkStruct_0x128* self, bool paused) {
    if (paused) {
        UnkSoundIdListNode* n = self->field_0x80.head;
        while (n) {
            UnkSoundIdRef ref = n->ref;
            if (self->paused.IndexOf(ref.ptr) < 0 && ref.ptr->state != 7) {
                fn_710021f950(reinterpret_cast<UnkMedia324*>(ref.ptr));
                self->paused.Add(ref.ptr);
            }
            n = n->next;
        }
    } else {
        UnkPausedNode_lf022c4* n = self->paused.head;
        while (n) {
            UnkSoundIdHandle_lf022c4 ref = n->ref;
            fn_710021fa20(reinterpret_cast<UnkMedia324*>(ref.ptr));
            n = n->next;
        }
        self->paused.ClearDestroying();
    }
}
