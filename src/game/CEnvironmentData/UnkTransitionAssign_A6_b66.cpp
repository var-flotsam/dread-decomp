// CEnvironmentData transition unit (tools/tu_map.py): assignment of a {u32, CVector4D, u64 at +0x14} record,
// skipped on self-assignment. Two copies. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkTransitionRec_A6 {
    u32 field_0x0;
    base::math::CVector4D color;
    u64 field_0x14;
} __attribute__((packed, aligned(4)));

static inline __attribute__((always_inline)) UnkTransitionRec_A6* Assign(UnkTransitionRec_A6* self, const UnkTransitionRec_A6* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        fn_710007b518(&self->color, &other->color);
        self->field_0x14 = other->field_0x14;
    }
    return self;
}

// Reflected assignments (slots 0x48/0x50 in src/reflection/class_descriptors_02.cpp).
void* fn_7100f62f24(void* dst, const void* src) {
    return Assign(static_cast<UnkTransitionRec_A6*>(dst), static_cast<const UnkTransitionRec_A6*>(src));
}
void* fn_7100f62f68(void* dst, const void* src) {
    return Assign(static_cast<UnkTransitionRec_A6*>(dst), static_cast<const UnkTransitionRec_A6*>(src));
}
