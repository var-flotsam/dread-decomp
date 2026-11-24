// Unit 2578 (no reflected class): the sound-id manager's reset (the 0x150-byte object at g_unk_7101d47320, whose layout
// src/game/UnkSoundIdRegistry.cpp describes): the instances playing under its ids are stopped (fn_710021fafc), the
// registrations removed (fn_7100f004dc) and the two coloured items released to the render manager after the
// last blend id (the stack at +0x128) is applied to them. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSoundIdInstance.h"
#include "game/UnkTU287.h"
#include "game/UnkTU324.h"
#include "types.h"
#include "base/UnkLinkedList.h"

struct UnkSoundIdRef_ovb0221 {
    UnkSoundIdInstance* ptr;

    UnkSoundIdRef_ovb0221(const UnkSoundIdRef_ovb0221& other) : ptr(other.ptr) {
        if (ptr)
            __atomic_fetch_add(&ptr->ref_count, 1, __ATOMIC_SEQ_CST);
    }
    ~UnkSoundIdRef_ovb0221() {
        if (UnkSoundIdInstance* p = ptr) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__builtin_expect(__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0, 1))
                p->vf_0x10();
        }
    }
};

struct UnkSoundIdListNode_ovb0221 {
    u32 key;
    UnkSoundIdRef_ovb0221 ref;
    UnkSoundIdListNode_ovb0221* next;
};

struct UnkRefObj_7100f011c4;
struct UnkSoundIdRegistry;
void fn_7100f004dc(UnkSoundIdRegistry* self);

struct UnkSoundIdItemHolder {
    u8 pad_0x0[0x18];
    u8 item[0x10];  // 0x18
};

struct UnkStruct_0x128 {
    u8 pad_0x0[0x80];
    base::UnkLinkedList<UnkSoundIdListNode_ovb0221> field_0x80;
    u8 pad_0xa8[0x60];
    UnkSoundIdItemHolder* field_0x108;
    UnkSoundIdItemHolder* field_0x110;
    u8 pad_0x118[0x10];
    u32* ids;       // 0x128
    u32 id_count;   // 0x130
};

void fn_7100f00600(UnkStruct_0x128* self) {
    UnkSoundIdListNode_ovb0221* node = self->field_0x80.head;
    while (node) {
        UnkSoundIdRef_ovb0221 ref = node->ref;
        fn_710021fafc(reinterpret_cast<UnkRefObj_7100f011c4*>(ref.ptr));
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
