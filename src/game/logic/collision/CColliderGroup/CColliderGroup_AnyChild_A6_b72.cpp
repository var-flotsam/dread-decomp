// game::logic::collision::CColliderGroup: whether any child collider in the inline list at +0x124 (a u32
// count followed directly by the pointers, at +0x128) answers true to its virtual at slot 0x68 for the
// argument; the count is re-read after every call. Names and layouts are placeholders.
#include "types.h"

struct UnkCollider_A6_b72 {
    void** vtable;
    bool vf_0x68(void* arg) {
        return reinterpret_cast<bool (*)(UnkCollider_A6_b72*, void*)>(vtable[0x68 / 8])(this, arg);
    }
};

#pragma pack(push, 4)
struct UnkColliderGroup_A6_b72 {
    u8 pad_0x0[0x124];
    u32 count;
    UnkCollider_A6_b72* children[1];  // 0x128, 4-byte aligned
};
#pragma pack(pop)

bool fn_7100d4590c(UnkColliderGroup_A6_b72* self, void* arg) {
    u64 begin = reinterpret_cast<u64>(self->children);
    for (u64 it = begin; it < begin + (u64)self->count * 8; it += 8)
        if ((*reinterpret_cast<UnkCollider_A6_b72**>(it))->vf_0x68(arg))
            return true;
    return false;
}
