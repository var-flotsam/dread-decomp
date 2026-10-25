// Destructors of node lists (count at 8, head and tail at 0x10/0x18, allocator at 0x20) whose
// nodes have destructors: each node is destroyed and freed, then the list is emptied.
// - fn_7100219134 (vtable unk_7101a202e0): nodes are an id and path pair (fn_710008d8e8), linked
//   through 0x10.
// - fn_710002e3f8 (unk_7101a1a070): nodes are a string (fn_710007ecd4), linked through 0x30.
// - fn_7100103cb8 (unk_7101a1bcf0): nodes run fn_710007ed40 and then the string destructor,
//   linked through 0x148.
// Also the clear of such a list with plain nodes (fn_710023fe88). fn_71011aaf80 is a relative.

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

extern char unk_7101a202e0[];
extern char unk_7101a1a070[];
extern char unk_7101a1bcf0[];
void fn_710008d8e8(char* node);
void fn_710007ed40(char* node);

struct UnkNodeList_7100219134 {
    char* vtable;
    u32 count;
    char* head;
    char* tail;
    base::UnkAllocator* allocator;
};

template <char* Vtable, u32 Next, void (*DestroyNode)(char*)>
static inline void DestroyList(UnkNodeList_7100219134* self) {
    self->vtable = Vtable + 0x10;
    for (char* n = self->head; n;) {
        char* next = *reinterpret_cast<char**>(n + Next);
        DestroyNode(n);
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

// A node that is a string (base/global/CRntString.h).
static inline void DestroyNode_710002e3f8(char* node) { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node)); }

static inline void DestroyNode_7100103cb8(char* node) {
    fn_710007ed40(node);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node));
}

void fn_7100219134(UnkNodeList_7100219134* self) { DestroyList<unk_7101a202e0, 0x10, fn_710008d8e8>(self); }

void fn_710002e3f8(UnkNodeList_7100219134* self) { DestroyList<unk_7101a1a070, 0x30, DestroyNode_710002e3f8>(self); }

void fn_7100103cb8(UnkNodeList_7100219134* self) {
    DestroyList<unk_7101a1bcf0, 0x148, DestroyNode_7100103cb8>(self);
}

// Clearing a list of plain nodes (linked through 8): fn_710023fe88.
void fn_710023fe88(UnkNodeList_7100219134* self) {
    for (char* n = self->head; n;) {
        char* next = *reinterpret_cast<char**>(n + 8);
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
