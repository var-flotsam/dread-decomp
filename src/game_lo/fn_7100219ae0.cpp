// Sets an entry's 4-bit state field to a new value when the owner's list knows that value and
// the state differs, then, for enabled entries, forwards the type-specific sub-object to the list.
#include "types.h"

// Per-type payload; each entry type has its own layout with the forwarded member at Offset.
template <int Type, int Offset>
struct UnkPayload_7100219ae0 {
    u8 pad[Offset];
    u8 member[8];
};

struct UnkEntry_7100219ae0 {
    u32 bits;
    u8 type_at_3() const { return ((const u8*)this)[3]; }
    u32 field_0x4;
    u64 field_0x8;
    void* field_0x10;

    template <int Type, int Offset>
    void* Member() const {
        return static_cast<UnkPayload_7100219ae0<Type, Offset>*>(field_0x10)->member;
    }
};

struct UnkOwner_7100219ae0 {
    u8 pad[0x220];
    void* field_0x220;
};

bool fn_71001fc69c(void* list, u32 value);
bool fn_7100217e50(u32 flags, u32 mask);
void fn_71001fd458(void* list, void* obj, u32 value);

bool fn_7100219ae0(UnkOwner_7100219ae0* self, UnkEntry_7100219ae0* entry, u32 value) {
    if (!fn_71001fc69c(self->field_0x220, value))
        return false;
    u32 bits = entry->bits;
    if (((bits >> 12) & 0xf) == value)
        return false;
    bits = (bits & ~0xf000u) | ((value & 0xf) << 12);
    entry->bits = bits;
    if (!fn_7100217e50(bits & 0x3fff, 1))
        return false;
    void* obj = nullptr;
    switch (entry->type_at_3()) {
#define C(t, o) case t: obj = entry->Member<t, o>(); break;
    C(3, 0x20) C(4, 0x20) C(5, 0x28) C(6, 0x28) C(8, 0x18) C(9, 0x18) C(10, 0x18) C(11, 0x18)
    C(12, 0x20) C(13, 0x20) C(14, 0x28) C(15, 0x20) C(16, 0x20) C(17, 0x20) C(18, 0x18)
    C(19, 0x20) C(21, 0x18) C(22, 0x18) C(23, 0x18) C(24, 0x28)
#undef C
    }
    fn_71001fd458(self->field_0x220, obj, value);
    return true;
}
