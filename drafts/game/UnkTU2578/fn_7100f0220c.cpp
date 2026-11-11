// Unit 2578: the sound-id manager (src/game/UnkSoundIdRegistry.cpp describes its layout; 0x150 bytes,
// g_unk_7101d47320). Names and layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/UnkVector.h"
#include "game/UnkRenderManager.h"
#include "types.h"

// A playing instance: reference-counted (atomically, at +8), destroyed through its vtable.
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

struct UnkSoundIdList {
    u32 count;
    UnkSoundIdListNode* head;
    UnkSoundIdListNode* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkSoundIdListNode* Find(u32 key) {
        for (UnkSoundIdListNode* n = head; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
};

struct UnkMedia324;
u32 fn_710021f950(UnkMedia324* self);
u32 fn_710021fa20(UnkMedia324* self);
void fn_710021fafc(UnkMedia324* self);

struct UnkStruct_0x128 {
    u8 pad_0x0[0x80];
    UnkSoundIdList field_0x80;
};


bool fn_7100f0220c(UnkStruct_0x128* self, u32 id) {
    UnkSoundIdListNode* n = self->field_0x80.Find(id);
    if (!n)
        return false;
    UnkSoundIdRef ref = n->ref;
    return ref.ptr->state == 0xd;
}

