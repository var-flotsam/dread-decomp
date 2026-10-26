// Tears the component down: runs the two base steps (the first with the hash 0xd20a8883f919f07e),
// removes the owner's object at +0x2a0 from its registry when it is in it, clears the pointers in
// the list at +0x78 (when the owner has a manager at +0x280), unregisters the counted reference at
// 0xb8 and releases it.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkSwitchObject.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCounted {
    void** vtable;
    u32 count;
    u32 pad_0xc;
    UnkWeakTarget* field_0x10;
    void vf_0x10() { reinterpret_cast<void (*)(UnkCounted*)>(vtable[2])(this); }
};

struct UnkOwner {
    u8 pad_0x0[0x280];
    void* field_0x280;
    u8 pad_0x288[0x18];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
    u8 pad_0x10[0x68];
    u8 field_0x78[8];
    u32 count;
    u8 pad_0x84[0x1c];
    u8 field_0xa0[0x18];
    UnkCounted* field_0xb8;
};

extern __attribute__((visibility("hidden"))) base::global::CStrId g_unk_71726c8da8;
void fn_710042bb40(UnkSelf* self);
void** fn_7100443d5c(void* list, u32 index);

void fn_71004468a8(UnkSelf* self) {
    fn_7100426b54(reinterpret_cast<UnkChozoCommanderAttack*>(self), 0xd20a8883f919f07eull, false);
    fn_710042bb40(self);
    UnkCollisionContainer* object = self->field_0x8->field_0x2a0;
    if (object) {
        if (fn_71008640a4(object, &g_unk_71726c8da8))
            fn_7100863a8c(object, &g_unk_71726c8da8);
    }
    if (self->field_0x8->field_0x280) {
        for (u32 i = 0; i < self->count; ++i) {
            if (void* entry = *fn_7100443d5c(self->field_0x78, i))
                fn_7100146e60(static_cast<UnkSwitchObject*>(entry), 0);
        }
        self->count = 0;
    }
    if (UnkCounted* ref = self->field_0xb8) {
        if (ref->field_0x10) {
            fn_710117b3f0(ref->field_0x10, self->field_0xa0);
            ref = self->field_0xb8;
            if (!ref)
                return;
        }
        if (--ref->count == 0)
            ref->vf_0x10();
        self->field_0xb8 = nullptr;
    }
}
