// The set holder of fn_7100db31a8 (src/game_71001e2acc/fn_71002f7850.cpp) as the second base of an
// object, at +0x8: its destructors entered with the holder's address (complete: fn_7100db2468;
// deleting, which frees the whole object: fn_7100db2514). Restore the holder's vtable, clear the
// set (fn_7100db3200) and free its storage. Names and the layout are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101aa93e8[];
void fn_7100db3200(void* set);

struct UnkSetHolderOuter {
    u64 field_0x0;
    char* vtable;
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x20[0x10];
    base::UnkAllocator* allocator;
};

struct UnkSetHolderInner {
    char* vtable;
    void* data;
};

__attribute__((always_inline)) inline UnkSetHolderOuter* OuterOf(UnkSetHolderInner* self) {
    return reinterpret_cast<UnkSetHolderOuter*>(reinterpret_cast<char*>(self) - 8);
}

template <bool ResetCapacity>
__attribute__((always_inline)) inline void DestroyHolder(UnkSetHolderInner* self, UnkSetHolderOuter* o) {
    self->vtable = unk_7101aa93e8 + 0x10;
    fn_7100db3200(&self->data);
    if (void* data = self->data) {
        o->count = 0;
        o->allocator->vf_0x48(data);
        if (ResetCapacity)
            o->capacity = 0;
    }
}

void fn_7100db2468(UnkSetHolderInner* self) {
    UnkSetHolderOuter* o = OuterOf(self);
    asm("" : "+r"(o));
    DestroyHolder<true>(self, o);
}

void fn_7100db2514(UnkSetHolderInner* self) {
    UnkSetHolderOuter* o = OuterOf(self);
    asm("" : "+r"(o));
    DestroyHolder<false>(self, o);
    operator delete(o);
}
