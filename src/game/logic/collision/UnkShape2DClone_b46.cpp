// Two copies of a 2D shape clone, one each in the game::logic::collision::CCapsuleShape2D and
// CAABoxShape2D units (tools/tu_map.py): a new 0xc0-byte shape (operator new, fn_7100083b60) built
// by the shape base constructor from the source's transform (fn_7100d6c034, from +0x8), the
// class's vtable, a cleared flag (+0xb8) and the source's four words at +0xa8. Names and layouts
// are placeholders.
#include "types.h"

struct UnkShape2D_b46a3 {
    void* vtable;
    u8 transform[0xa0];  // 0x8
    u32 field_0xa8;
    u32 field_0xac;
    u32 field_0xb0;
    u32 field_0xb4;
    bool field_0xb8;
};

void fn_7100d6c034(UnkShape2D_b46a3* self, const void* transform);

template <char* Vtable>
static inline UnkShape2D_b46a3* Clone(const UnkShape2D_b46a3* other) {
    auto* self = static_cast<UnkShape2D_b46a3*>(__builtin_operator_new(0xc0));
    fn_7100d6c034(self, other->transform);
    self->vtable = Vtable + 0x10;
    self->field_0xb8 = false;
    self->field_0xa8 = other->field_0xa8;
    self->field_0xac = other->field_0xac;
    self->field_0xb0 = other->field_0xb0;
    self->field_0xb4 = other->field_0xb4;
    return self;
}

extern char g_unk_7101aa8970[];  // CCapsuleShape2D's vtable
extern char g_unk_7101aa88c0[];  // CAABoxShape2D's vtable

UnkShape2D_b46a3* fn_7100d601c8(const UnkShape2D_b46a3* other) { return Clone<g_unk_7101aa8970>(other); }
UnkShape2D_b46a3* fn_7100d5e4c4(const UnkShape2D_b46a3* other) { return Clone<g_unk_7101aa88c0>(other); }
