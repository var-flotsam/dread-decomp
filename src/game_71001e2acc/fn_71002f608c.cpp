// Destructor of a node list (count at 8, head and tail at 0x10/0x18, allocator at 0x20) whose nodes
// run fn_710007ed40 and then the string destructor (as in fn_7100103cb8,
// src/game_s1/fn_7100219134.cpp), linked through 0x68, with vtable unk_7101a222c8.

#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101a222c8[];
void fn_710007ecd4(char* node);
void fn_710007ed40(char* node);

struct UnkNodeList_71002f608c {
    char* vtable;
    u32 count;
    char* head;
    char* tail;
    base::UnkAllocator* allocator;
};

void fn_71002f608c(UnkNodeList_71002f608c* self) {
    self->vtable = unk_7101a222c8 + 0x10;
    for (char* n = self->head; n;) {
        char* next = *reinterpret_cast<char**>(n + 0x68);
        fn_710007ed40(n);
        fn_710007ecd4(n);
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
