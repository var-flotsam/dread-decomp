// navmesh::SNavMeshPath's unit (tools/tu_map.py unit 366): two loops that call the virtual at +0x30
// of every 0x120-byte element of an inline array (count at +0x18, elements from +0x1c; count at
// +0x8, elements from +0xc) with one argument. Names and layouts are placeholders.
#include "types.h"

struct UnkNavElem_b46a1 {
    struct VTable {
        void* slots[6];
        void (*vf_0x30)(UnkNavElem_b46a1* self, void* arg);
    };
    VTable* vtable;
    u8 pad_0x8[0x118];
};

template <u32 CountOffset>
static inline __attribute__((always_inline)) void ForEach(char* self, void* arg) {
    u32 count = *reinterpret_cast<u32*>(self + CountOffset);
    UnkNavElem_b46a1* it = reinterpret_cast<UnkNavElem_b46a1*>(self + CountOffset + 4);
    for (UnkNavElem_b46a1* end = it + count; it != end; ++it)
        it->vtable->vf_0x30(it, arg);
}

void fn_71002adef0(char* self, void* arg) { ForEach<0x18>(self, arg); }
void fn_71002ac5cc(char* self, void* arg) { ForEach<0x8>(self, arg); }
