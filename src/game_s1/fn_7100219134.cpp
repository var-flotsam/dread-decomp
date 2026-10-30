// Destructors of node lists (count at 8, head and tail at 0x10/0x18, allocator at 0x20) whose
// nodes have destructors: each node is destroyed and freed, then the list is emptied.
// - fn_7100219134 (vtable unk_7101a202e0): nodes are an id and path pair (fn_710008d8e8), linked
//   through 0x10.
// - fn_710002e3f8 (unk_7101a1a070): nodes are a string (fn_710007ecd4), linked through 0x30.
// - fn_7100103cb8 (unk_7101a1bcf0): nodes run fn_710007ed40 and then the string destructor,
//   linked through 0x148.
// Also the clear of such a list with plain nodes (fn_710023fe88). fn_71011aaf80 is a relative.

#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

extern char unk_7101a202e0[];
extern char unk_7101a1a070[];
extern char unk_7101a1bcf0[];

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

// A node that is an id and path pair.
static inline void DestroyNode_7100219134(char* node) { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(node)); }

// A node that is a string (base/global/CRntString.h).
static inline void DestroyNode_710002e3f8(char* node) { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node)); }

static inline void DestroyNode_7100103cb8(char* node) {
    auto* str = reinterpret_cast<base::global::CRntString*>(node);
    fn_710007ed40(str);
    fn_710007ecd4(str);
}

void fn_7100219134(UnkNodeList_7100219134* self) { DestroyList<unk_7101a202e0, 0x10, DestroyNode_7100219134>(self); }

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

// Identical copies: fn_7100d82e8c, fn_71011d2a74.
void fn_7100d82e8c(UnkNodeList_7100219134* self) {
    for (char* n = self->head; n;) {
        char* next = *reinterpret_cast<char**>(n + 8);
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
void fn_71011d2a74(UnkNodeList_7100219134* self) {
    for (char* n = self->head; n;) {
        char* next = *reinterpret_cast<char**>(n + 8);
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

// The same destructor in three other units, for lists linked through 0x88, 0x120 and 0x20, with their
// own vtables and node destructors.
#include "base/UnkEmptyDtorElem.h"

extern char unk_7101a6f558[];
extern char unk_7101aa4fb0[];
extern char unk_7101cb2618[];
void fn_7100431cec(void* node);
void fn_7100d62bd8(void* node);

static inline void DestroyNode_7100790f50(char* node) { fn_7100431cec(node); }
static inline void DestroyNode_7100c75d5c(char* node) { fn_7100d62bd8(node); }
static inline void DestroyNode_7101115e70(char* node) { fn_71002874dc(node); }

void fn_7100790f50(UnkNodeList_7100219134* self) { DestroyList<unk_7101a6f558, 0x88, DestroyNode_7100790f50>(self); }

void fn_7100c75d5c(UnkNodeList_7100219134* self) { DestroyList<unk_7101aa4fb0, 0x120, DestroyNode_7100c75d5c>(self); }

void fn_7101115e70(UnkNodeList_7100219134* self) { DestroyList<unk_7101cb2618, 0x20, DestroyNode_7101115e70>(self); }
