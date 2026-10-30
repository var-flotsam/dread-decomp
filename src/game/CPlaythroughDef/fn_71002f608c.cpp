// Destructor of a node list (count at 8, head and tail at 0x10/0x18, allocator at 0x20) whose nodes
// run fn_710007ed40 and then the string destructor (as in fn_7100103cb8,
// src/game_s1/fn_7100219134.cpp), linked through 0x68, with vtable unk_7101a222c8.

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

extern char unk_7101a222c8[];

struct UnkNodeList_71002f608c {
    char* vtable;
    u32 count;
    char* head;
    char* tail;
    base::UnkAllocator* allocator;
};

template <char* Vtable, u32 Next>
__attribute__((always_inline)) inline void DestroyStrList(UnkNodeList_71002f608c* self) {
    self->vtable = Vtable + 0x10;
    for (char* n = self->head; n;) {
        char* next = *reinterpret_cast<char**>(n + Next);
        auto* str = reinterpret_cast<base::global::CRntString*>(n);
        fn_710007ed40(str);
        fn_710007ecd4(str);
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

void fn_71002f608c(UnkNodeList_71002f608c* self) {
    DestroyStrList<unk_7101a222c8, 0x68>(self);
}

// The base-part destructors of the string lists of the CCharClassFrozenAsPlatformComponent and
// CMeleeComponent units (base/UnkNodeOwner.h has their derived parts), linked through 0xc8 and 0x88.
extern char unk_7101a74478[];
extern char unk_7101a838e8[];

void fn_71007fd4c8(UnkNodeList_71002f608c* self) {
    DestroyStrList<unk_7101a74478, 0xc8>(self);
}

void fn_7100932010(UnkNodeList_71002f608c* self) {
    DestroyStrList<unk_7101a838e8, 0x88>(self);
}
