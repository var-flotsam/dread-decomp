// Deleting destructor of a reflection class holding a chained hash set at 8 (as in fn_7100022518,
// src/game_s1/fn_7100022518.cpp): vtable unk_7101a22398, clear fn_71002f6ac0; and five copies, each
// with its own vtable and clear.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101a22398[];
void fn_71002f6ac0(void* set);

struct UnkSetHolder_71002f7850 {
    char* vtable;
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x18[0x10];
    base::UnkAllocator* allocator;
};

template <char* Vtable, class Set, void (*Clear)(Set*)>
__attribute__((always_inline)) inline void Destroy(UnkSetHolder_71002f7850* self) {
    self->vtable = Vtable + 0x10;
    Clear((Set*)&self->data);
    if (self->data) {
        self->count = 0;
        self->allocator->vf_0x48(self->data);
    }
    operator delete(self);
}

void fn_71002f7850(UnkSetHolder_71002f7850* self) {
    Destroy<unk_7101a22398, void, fn_71002f6ac0>(self);
}

extern char unk_7101aa9020[];
extern char unk_7101aa9080[];
extern char unk_7101aa90e0[];
extern char unk_7101aa93e8[];
extern char unk_7101cac6e8[];
void fn_7100d94aa8(void* set);
void fn_7100d957fc(void* set);
void fn_7100d96550(void* set);
void fn_7100db3200(void* set);
void fn_710102be04(base::UnkVector<u64>* set);  // as src/game/sound/CSoundManager/fn_71010233ec.cpp declares it

void fn_7100d943f0(UnkSetHolder_71002f7850* self) {
    Destroy<unk_7101aa9020, void, fn_7100d94aa8>(self);
}

void fn_7100d95144(UnkSetHolder_71002f7850* self) {
    Destroy<unk_7101aa9080, void, fn_7100d957fc>(self);
}

void fn_7100d95e98(UnkSetHolder_71002f7850* self) {
    Destroy<unk_7101aa90e0, void, fn_7100d96550>(self);
}

void fn_7100db31a8(UnkSetHolder_71002f7850* self) {
    Destroy<unk_7101aa93e8, void, fn_7100db3200>(self);
}

void fn_7101028690(UnkSetHolder_71002f7850* self) {
    Destroy<unk_7101cac6e8, base::UnkVector<u64>, fn_710102be04>(self);
}
