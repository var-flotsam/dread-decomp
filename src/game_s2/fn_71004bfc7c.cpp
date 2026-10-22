// Copy assignments (fn_71004bfc7c, and fn_71004b206c with a u32 instead of the byte) of a
// two-field record stored either in place or, when the kind at 0x20 is 2, behind a pointer in
// each slot: the u32 field and the u64 and tail value are copied between them.
#include "types.h"

template <class Tail>
struct UnkBlock {
    u8 unk_0x0[8];
    u64 field_0x8;
    Tail field_0x10;
};

struct UnkBoxed {
    u8 unk_0x0[0x20];
    s32 kind;

    // The first field lives at 0 (or behind the pointer there), the block at 8.
    u32* First() { return kind == 2 ? *reinterpret_cast<u32**>(this) : reinterpret_cast<u32*>(this); }
    template <class Tail>
    UnkBlock<Tail>* Second() {
        u8* slot = reinterpret_cast<u8*>(this) + 8;
        return kind == 2 ? *reinterpret_cast<UnkBlock<Tail>**>(slot)
                         : reinterpret_cast<UnkBlock<Tail>*>(slot);
    }
};

template <class Tail>
static inline void Assign(UnkBoxed* self, UnkBoxed* other) {
    if (self == other)
        return;
    *self->First() = *other->First();
    UnkBlock<Tail>* src = other->Second<Tail>();
    UnkBlock<Tail>* dst = self->Second<Tail>();
    u64 a = src->field_0x8;
    Tail b = src->field_0x10;
    dst->field_0x10 = b;
    dst->field_0x8 = a;
}

void fn_71004bfc7c(UnkBoxed* self, UnkBoxed* other) { Assign<u8>(self, other); }
void fn_71004b206c(UnkBoxed* self, UnkBoxed* other) { Assign<u32>(self, other); }
