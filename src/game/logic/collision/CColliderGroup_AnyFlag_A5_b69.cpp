// game::logic::collision::CColliderGroup: its flag at +0x6b becomes whether any child collider in the
// inline list at +0x124 (a u32 count followed directly by the pointers, at +0x128) has the same flag
// set. The group is a collider itself, so the flag is re-stored after every child. Names and layouts
// are placeholders.
#include "types.h"

struct UnkColliderBase_A5_b69 {
    u8 pad_0x0[0x6b];
    bool flag;
};

#pragma pack(push, 4)
struct UnkColliderGroup_A5_b69 : UnkColliderBase_A5_b69 {
    u8 pad_0x6c[0x124 - 0x6c];
    u32 count;
    UnkColliderBase_A5_b69* children[1];  // 0x128, 4-byte aligned
};
#pragma pack(pop)

void fn_7100d45a28(UnkColliderGroup_A5_b69* self) {
    u64 bytes = (u64)self->count * sizeof(void*);
    asm("" : "+r"(bytes));
    self->flag = false;
    for (UnkColliderBase_A5_b69 **it = self->children, **end = (UnkColliderBase_A5_b69**)((char*)it + bytes); it != end;
         ++it)
        self->flag = self->flag || (*it)->flag;
}
