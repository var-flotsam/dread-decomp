// Destructor of a pair of a file-path id and an object over the 0x58-byte base (fn_7100016f88):
// the object restores its vtable, clears (fn_7100da0870) and frees its array, releases its own
// file-path id and runs the base destructor; then the outer id is released. Three identical
// copies.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkBase_0x58 {
    void** vtable;
    u8 unk_0x8[0x50];
    ~UnkBase_0x58() asm("fn_7100016f88");  // fn_7100016f88
};

struct UnkClearedArray;
void fn_7100da0870(UnkClearedArray* self, u32 count);  // resizes

// base::UnkVector's layout; cleared out of line before its storage is freed.
struct UnkClearedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkClearedArray() {
        if (data) {
            fn_7100da0870(this, 0);
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

extern void* g_unk_7101aa9140[];

struct UnkPathArrayObj : UnkBase_0x58 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    UnkClearedArray array;

    __attribute__((always_inline)) ~UnkPathArrayObj() { vtable = &g_unk_7101aa9140[2]; }
};

struct UnkPathObjPair {
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    UnkPathArrayObj obj;

    __attribute__((always_inline)) ~UnkPathObjPair() {}
};

void fn_7100e0ccf8(UnkPathObjPair* self) { self->~UnkPathObjPair(); }
void fn_7100e0d204(UnkPathObjPair* self) { self->~UnkPathObjPair(); }
void fn_7100e0da38(UnkPathObjPair* self) { self->~UnkPathObjPair(); }
