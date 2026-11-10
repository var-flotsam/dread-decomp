// Unit 2578 (no reflected class): the sound-id manager's reset (the 0x150-byte object at g_unk_7101d47320, whose layout
// src/game/UnkSoundIdRegistry.cpp describes): the instances playing under its ids are stopped (fn_710021fafc), the
// registrations removed (fn_7100f004dc) and the two coloured items released to the render manager after the
// last blend id (the stack at +0x128) is applied to them. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkRenderManager.h"
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

struct UnkSoundIdList {
    u32 count;
    UnkSoundIdListNode* head;
    UnkSoundIdListNode* tail;
    base::UnkAllocator* allocator;
    u32 flags;
};

struct UnkRefObj;
void fn_710021fafc(UnkRefObj* self);
struct UnkSoundIdRegistry;
void fn_7100f004dc(UnkSoundIdRegistry* self);
void fn_710020ad90(void* self, s32 value);

struct UnkSoundIdItemHolder {
    u8 pad_0x0[0x18];
    u8 item[0x10];  // 0x18
};

struct UnkStruct_0x128 {
    u8 pad_0x0[0x80];
    UnkSoundIdList field_0x80;
    u8 pad_0xa8[0x60];
    UnkSoundIdItemHolder* field_0x108;
    UnkSoundIdItemHolder* field_0x110;
    u8 pad_0x118[0x10];
    u32* ids;       // 0x128
    u32 id_count;   // 0x130
};

void fn_7100f00600(UnkStruct_0x128* self) {
    UnkSoundIdListNode* node = self->field_0x80.head;
    while (node) {
        UnkSoundIdRef ref = node->ref;
        fn_710021fafc(reinterpret_cast<UnkRefObj*>(ref.ptr));
        node = node->next;
    }
    fn_7100f004dc(reinterpret_cast<UnkSoundIdRegistry*>(self));
    if (self->field_0x108) {
        u32 n = self->id_count;
        if (n) {
            self->id_count = --n;
            if (n) {
                u32 id = (self->ids + n)[-1];
                fn_710020ad90(self->field_0x108->item, id);
                fn_710020ad90(self->field_0x110->item, id);
            }
        }
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(self->field_0x108), 2);
        self->field_0x108 = nullptr;
    }
    if (self->field_0x110) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(self->field_0x110), 2);
        self->field_0x110 = nullptr;
    }
}
