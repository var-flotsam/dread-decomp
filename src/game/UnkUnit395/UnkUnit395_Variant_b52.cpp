// Copies of fn_710075ebd8 (src/game/CCharClassRodomithonXAIComponent/
// CCharClassRodomithonXAIComponent_Variant_b49a3.cpp) in unit 395 of tools/tu_map.py (slot 0x38 of
// two reflected classes): a small tagged value (kind at +0x18) built from another; kind 1 holds a
// 4-byte value and 16 bytes, kind 2 holds pointers to them; the copy always holds kind 1, and any
// other kind gives kind 0. Names and layouts are placeholders.
#include "types.h"

namespace {

struct UnkVariantBlock_b52 {
    u64 lo;
    u64 hi;
};

struct UnkVariantValue_b52 {
    u32 head;
    u32 body[4];
};

struct UnkVariantRef_b52 {
    const u32* head;
    const UnkVariantBlock_b52* body;
};

struct UnkVariant_b52 {
    union {
        UnkVariantValue_b52 value;
        UnkVariantRef_b52 ref;
    };
    u32 kind;  // 0x18
};

}  // namespace

__attribute__((always_inline)) static inline void UnkVariant_b52_Copy(void* dst, const void* src) {
    UnkVariant_b52* self = static_cast<UnkVariant_b52*>(dst);
    const UnkVariant_b52* other = static_cast<const UnkVariant_b52*>(src);
    self->kind = 0;
    switch (other->kind) {
    case 1:
        self->kind = other->kind;
        self->value = other->value;
        break;
    case 2: {
        self->kind = 1;
        UnkVariantRef_b52 ref = other->ref;
        self->value.head = *ref.head;
        __builtin_memcpy(self->value.body, ref.body, sizeof(UnkVariantBlock_b52));
        break;
    }
    }
}

void fn_71002cb64c(void* dst, const void* src) { UnkVariant_b52_Copy(dst, src); }
void fn_71002cb8a8(void* dst, const void* src) { UnkVariant_b52_Copy(dst, src); }
