// Unit 27 (tools/tu_map.py): grows an array of counted resource references to `capacity` elements
// (base::global::CRntVector's Reserve written out, as fn_710117c2e4 in
// src/game/CAnimationComponent/CAnimationComponent_b52.cpp). The signature is the one
// src/engine/UnkGraphicsSystem.cpp declares. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "game/UnkCountedRef_b53.h"
#include "types.h"

void ReserveCountedRefVector(base::global::CRntVector<u8>* vector, u32 capacity) {
    auto* self = reinterpret_cast<base::global::CRntVector<UnkCountedRef_b53>*>(vector);
    if (self->capacity < capacity) {
        auto* p = static_cast<UnkCountedRef_b53*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkCountedRef_b53), 8, self->flags));
        if (self->data) {
            UnkCountedRef_b53* src = self->data;
            UnkCountedRef_b53* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkCountedRef_b53(static_cast<UnkCountedRef_b53&&>(*src));
                src->~UnkCountedRef_b53();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}
