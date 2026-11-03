// A small tagged value (kind at +0x18) built from another: kind 1 holds a 4-byte value and 16 bytes,
// kind 2 holds pointers to them; the copy always holds kind 1 (a copy of the values), and any other kind
// gives kind 0. The same code is in CCharClassRodomithonXAIComponent's unit (987: fn_71006678f0,
// fn_7100667b4c) and CBreakableTileGroupComponent's unit (1167: fn_710075e97c, fn_710075ebd8), one per
// value type. Names and layouts are placeholders.
#include "types.h"

namespace {

struct UnkVariantBlock_b49a3 {
    u64 lo;
    u64 hi;
};

struct UnkVariantValue_b49a3 {
    u32 head;
    u32 body[4];
};

struct UnkVariantRef_b49a3 {
    const u32* head;
    const UnkVariantBlock_b49a3* body;
};

}  // namespace

struct UnkVariant_b49a3 {
    union {
        UnkVariantValue_b49a3 value;
        UnkVariantRef_b49a3 ref;
    };
    u32 kind;  // 0x18
};

__attribute__((always_inline)) static inline void UnkVariant_b49a3_Copy(UnkVariant_b49a3* self,
                                                                         const UnkVariant_b49a3* other) {
    self->kind = 0;
    switch (other->kind) {
    case 1:
        self->kind = other->kind;
        self->value = other->value;
        break;
    case 2: {
        self->kind = 1;
        UnkVariantRef_b49a3 ref = other->ref;
        self->value.head = *ref.head;
        __builtin_memcpy(self->value.body, ref.body, sizeof(UnkVariantBlock_b49a3));
        break;
    }
    }
}

void fn_71006678f0(UnkVariant_b49a3* self, const UnkVariant_b49a3* other) { UnkVariant_b49a3_Copy(self, other); }
void fn_7100667b4c(UnkVariant_b49a3* self, const UnkVariant_b49a3* other) { UnkVariant_b49a3_Copy(self, other); }
void fn_710075e97c(UnkVariant_b49a3* self, const UnkVariant_b49a3* other) { UnkVariant_b49a3_Copy(self, other); }
void fn_710075ebd8(UnkVariant_b49a3* self, const UnkVariant_b49a3* other) { UnkVariant_b49a3_Copy(self, other); }
