// Unit 958 (tools/tu_map.py, no name hint). fn_710063c0bc: whether any active entry (flag at +0x28) of
// the array at +0x8 (count at +0x10, 0x30-byte entries) has no object (+0x20) or an object whose
// +0x10 is null. Names and layouts are placeholders.
#include "types.h"

struct UnkObject_958_b43a2 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkEntry_958_b43a2 {
    u8 pad_0x0[0x20];
    UnkObject_958_b43a2* object;  // 0x20
    bool active;  // 0x28
    u8 pad_0x29[0x7];
};

struct UnkHolder_958_b43a2 {
    u8 pad_0x0[0x8];
    UnkEntry_958_b43a2* data;  // 0x8
    u32 count;  // 0x10
};

// Signature as src/game/behaviortree/CIsThereFreeOffsetCondition/CIsThereFreeOffsetCondition.cpp declares it.
bool fn_710063c0bc(u8* part) {
    UnkHolder_958_b43a2* self = reinterpret_cast<UnkHolder_958_b43a2*>(part);
    for (UnkEntry_958_b43a2 *p = self->data, *end = p + self->count; p != end; ++p) {
        if (p->active && (!p->object || !p->object->field_0x10))
            return true;
    }
    return false;
}
