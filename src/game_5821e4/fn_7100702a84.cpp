// CAIGotoPointNavigationState::SEntityTarget (reflection: size 0x20, parent SPointTarget): its
// copy constructor (fn_7100702a84) and move constructor (fn_7100702b18), both the implicit copy.
#include "CAttack.h"

#include <new>

// Three floats copied one by one without type-based alias info (each a 4-byte memcpy), so the
// store of the base vtable before them stays.
struct UnkVec3_7100702a84 {
    f32 x, y, z;

    UnkVec3_7100702a84(const UnkVec3_7100702a84& other) {
        __builtin_memcpy(&x, &other.x, 4);
        __builtin_memcpy(&y, &other.y, 4);
        __builtin_memcpy(&z, &other.z, 4);
    }
};

struct SPointTarget {
    virtual ~SPointTarget();
    UnkVec3_7100702a84 field_0x8;
};

struct SEntityTarget : SPointTarget {
    ~SEntityTarget() override;
    UnkAttackRefPtr field_0x18;
};

void fn_7100702a84(SEntityTarget* self, const SEntityTarget* other) {
    new (self) SEntityTarget(*other);
}

void fn_7100702b18(SEntityTarget* self, const SEntityTarget* other) {
    new (self) SEntityTarget(*other);
}
