// From unit 83 of tools/tu_map.py (0x71000a4598-0x71000e5f9c, base::reflection by its names): the
// out-of-line copy constructor of the engine's array of 3D vectors (base::UnkVector<UnkVec3f>,
// 12-byte elements copied word by word). Records in src/game_aca57c/ copy their arrays through it.
#include "base/UnkVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The callers' name for base::UnkVector<UnkVec3f> (src/game_aca57c/fn_7100e509a4.cpp).
struct UnkPart48;

// Takes the source's settings and allocator, reserves its capacity and appends its elements.
__attribute__((flatten)) void fn_71000cbae4(UnkPart48* self_, const UnkPart48* other_) {
    auto* self = reinterpret_cast<base::UnkVector<UnkVec3f>*>(self_);
    auto* other = reinterpret_cast<const base::UnkVector<UnkVec3f>*>(other_);
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count != 0) {
        const UnkVec3f* end = other->data + other->count;
        for (const UnkVec3f* p = other->data; p != end; ++p)
            self->PushBackCopy(*p);
    }
}

// An identical copy in unit 108 (0x7100110c54-0x71001239b0), whose callers name the array UnkPart0
// (src/game_aca57c/fn_7100e509a4.cpp).
struct UnkPart0;
__attribute__((flatten)) void fn_710011d8b4(UnkPart0* self_, const UnkPart0* other_) {
    auto* self = reinterpret_cast<base::UnkVector<UnkVec3f>*>(self_);
    auto* other = reinterpret_cast<const base::UnkVector<UnkVec3f>*>(other_);
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count != 0) {
        const UnkVec3f* end = other->data + other->count;
        for (const UnkVec3f* p = other->data; p != end; ++p)
            self->PushBackCopy(*p);
    }
}
