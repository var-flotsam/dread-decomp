// Unit 2578 (no reflected class): the sound-id manager's (the 0x150-byte object at g_unk_7101d47320, whose
// layout src/game/UnkSoundIdRegistry.cpp describes) two calls on the instance playing under an id (the list at
// +0x80): fn_710021f950 and fn_710021fa20 of the media object (src/game/media/UnkMovieUnit324/), made while the
// instance is held. Names and layouts are placeholders.
#include "game/UnkTU324.h"
#include "types.h"

// A playing instance: reference-counted (atomically, at +8), destroyed through its vtable.
struct UnkSoundIdInstance {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
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
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
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

    UnkSoundIdListNode* Find(u32 key) {
        for (UnkSoundIdListNode* n = head; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
};

struct UnkMedia324;
u32 fn_710021fa20(UnkMedia324* self);

struct UnkStruct_0x128 {
    u8 pad_0x0[0x80];
    UnkSoundIdList field_0x80;
};

void fn_7100f020d4(UnkStruct_0x128* self, u32 id) {
    if (UnkSoundIdListNode* n = self->field_0x80.Find(id)) {
        UnkSoundIdRef ref = n->ref;
        fn_710021f950(reinterpret_cast<UnkMedia324*>(ref.ptr));
    }
}

void fn_7100f02170(UnkStruct_0x128* self, u32 id) {
    if (UnkSoundIdListNode* n = self->field_0x80.Find(id)) {
        UnkSoundIdRef ref = n->ref;
        fn_710021fa20(reinterpret_cast<UnkMedia324*>(ref.ptr));
    }
}
