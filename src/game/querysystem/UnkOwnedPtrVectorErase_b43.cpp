// More copies of fn_710036828c (src/game/querysystem/UnkOwnedPtrVectorErase_b41.cpp), an erase by index
// from a base::UnkVector of owning pointers, in further units (two in base::reflection's, one in
// GUI::CHUD's; fn_7100fb6c80 destroys each entry through its deleting destructor instead). Names and layouts are placeholders.
#include "game_s2/UnkOwnedList.h"
#include "types.h"

namespace {

// Moves the pointer in `src` into the empty slot `dst`, leaving `src` empty.
inline void MoveInto_b43a3(UnkOwnedPtr* dst, UnkOwnedPtr* src) {
    UnkOwned* t = src->ptr;
    src->ptr = nullptr;
    dst->ptr = t;
}

}  // namespace

struct UnkOwnedPtrVector_b43a3 : base::UnkVector<UnkOwnedPtr> {};

static inline __attribute__((always_inline)) void Erase_b43a3(UnkOwnedPtrVector_b43a3* self, const u32* index) {
    u32 at = *index;
    if (at < self->count) {
        u64 i = at;
        self->data[i].~UnkOwnedPtr();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedPtr* cur = &self->data[i];
            MoveInto_b43a3(cur, &cur[1]);
            (&self->data[i])[1].~UnkOwnedPtr();
        }
        self->count = self->count - 1;
    }
}

#define UNK_ERASE_B43A3(addr) \
    void fn_##addr(UnkOwnedPtrVector_b43a3* self, const u32* index) { Erase_b43a3(self, index); }

UNK_ERASE_B43A3(7100fa8b9c)
UNK_ERASE_B43A3(71011e71f0)

// The same erase where each entry is destroyed through vtable slot 0x8 .
struct UnkOwned8_b43a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkOwned8Ptr_b43a3 {
    union {
        UnkOwned8_b43a3* ptr;
    };
    ~UnkOwned8Ptr_b43a3() {
        UnkOwned8_b43a3* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x8();
    }
};

struct UnkOwned8PtrVector_b43a3 : base::UnkVector<UnkOwned8Ptr_b43a3> {};

// Moves the pointer in `src` into the empty slot `dst`, leaving `src` empty.
inline void MoveInto8_b43a3(UnkOwned8Ptr_b43a3* dst, UnkOwned8Ptr_b43a3* src) {
    UnkOwned8_b43a3* t = src->ptr;
    src->ptr = nullptr;
    dst->ptr = t;
}

static inline __attribute__((always_inline)) void Erase8_b43a3(UnkOwned8PtrVector_b43a3* self, const u32* index) {
    u32 at = *index;
    if (at < self->count) {
        u64 i = at;
        self->data[i].~UnkOwned8Ptr_b43a3();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwned8Ptr_b43a3* cur = &self->data[i];
            MoveInto8_b43a3(cur, &cur[1]);
            (&self->data[i])[1].~UnkOwned8Ptr_b43a3();
        }
        self->count = self->count - 1;
    }
}

void fn_7100fb6c80(UnkOwned8PtrVector_b43a3* self, const u32* index) { Erase8_b43a3(self, index); }
