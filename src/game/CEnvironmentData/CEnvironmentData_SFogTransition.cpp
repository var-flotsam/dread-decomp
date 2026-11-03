// Copy assignments of a 0x30-byte transition record in CEnvironmentData::SFogTransition's unit (two
// identical copies): 0x18 plain bytes, a vector at +0x18 assigned out of line (fn_710007b518),
// then a word and a byte (the byte read first). The shape of fn_71005b7568 (src/flag_id/fn_71005b7568.cpp). Names and
// layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkTransitionBlock_0x18 {
    u64 field_0x0, field_0x8, field_0x10;
};

struct UnkFogTransition {
    UnkTransitionBlock_0x18 field_0x0;
    base::math::CVector4D field_0x18;
    u32 field_0x28;
    u8 field_0x2c;
};

__attribute__((always_inline)) static inline UnkFogTransition* Assign(UnkFogTransition* self,
                                                                      const UnkFogTransition* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        fn_710007b518(&self->field_0x18, &other->field_0x18);
        u8 b = other->field_0x2c;
        self->field_0x28 = other->field_0x28;
        self->field_0x2c = b;
    }
    return self;
}

UnkFogTransition* fn_7100f73a84(UnkFogTransition* self, const UnkFogTransition* other) { return Assign(self, other); }
UnkFogTransition* fn_7100f73adc(UnkFogTransition* self, const UnkFogTransition* other) { return Assign(self, other); }
