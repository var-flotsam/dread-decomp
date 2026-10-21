// Returns an object to its fixed pool (0x4000 slots): clears it, releases its buffer and its
// up to 16 child objects (each back to their own pool), then swap-removes it from the pool's
// list of live objects. Null, an empty pool or an object not in the pool do nothing.
#include "types.h"

struct UnkFlags_7100161b90 {
    u32 bits;
    UnkFlags_7100161b90() : bits(0) {}
    UnkFlags_7100161b90(const UnkFlags_7100161b90& o) : bits(o.bits) {}
};

inline const UnkFlags_7100161b90& UnkFlagsNone_7100161b90() {
    static UnkFlags_7100161b90 s_none;
    return s_none;
}

struct UnkChild_7100161b90 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
    u32 field_0x28;
    u32 index;
};

struct UnkObj_7100161b90 {
    u64 field_0x0;
    u64 field_0x8;
    void* buffer;
    u32 field_0x18;
    u8 pad_0x1c[4];
    u32 index;
    u32 flags;
    UnkFlags_7100161b90 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    bool field_0x34;
    UnkChild_7100161b90* children[16];
    u8 pad_0xb8[0xc0];
    u8 field_0x178[1];
};

struct UnkHeap_7100161b90;

extern UnkObj_7100161b90* unk_716e2aadf8[0x4000];  // the pool's live objects
extern u32 unk_716e2cadf8;                         // their count
extern UnkChild_7100161b90* unk_716e2cae08[];      // the children's live objects
extern u32 unk_716e34ae08;                         // their count
extern UnkObj_7100161b90* unk_716e34ae10;          // a current object, cleared when it goes
extern UnkHeap_7100161b90* unk_716e34ae20;         // the buffer heap for flagged objects
extern UnkHeap_7100161b90* unk_716e34ae28;         // the buffer heap for the others
extern void (*unk_716e29b078)(void* block);        // resets the block at +0x178

void fn_7100153a88(UnkHeap_7100161b90* heap, void* ptr);  // frees through the heap

// NON_MATCHING(minor): the child loop's offset counter is compared in 64 bits (32 in the
// original); otherwise identical.
void fn_7100161b90(UnkObj_7100161b90* obj) {
    if (!obj)
        return;
    if (!unk_716e2cadf8)
        return;
    u32 index = obj->index;
    if (index >= 0x4000)
        return;
    if (unk_716e34ae10 == obj)
        unk_716e34ae10 = nullptr;
    obj->field_0x34 = false;
    obj->field_0x0 = 0;
    obj->field_0x8 = 0;
    void* block = obj->field_0x178;
    unk_716e29b078(block);
    fn_7100153a88(!(obj->flags & 1) ? unk_716e34ae28 : unk_716e34ae20, obj->buffer);
    obj->buffer = nullptr;
    for (s32 i = 0; i < 16; ++i) {
        UnkChild_7100161b90* c = obj->children[i];
        if (!c)
            continue;
        c->field_0x8 = 0;
        c->field_0x10 = 0;
        c->field_0x18 = 0;
        c->field_0x1c = 0x16;
        c->field_0x20 = 0;
        u32 ci = c->index;
        u32 last = --unk_716e34ae08;
        UnkChild_7100161b90* moved = unk_716e2cae08[last];
        unk_716e2cae08[ci] = moved;
        moved->index = ci;
        unk_716e2cae08[last] = c;
        c->index = 0xffffffff;
    }
    for (s32 i = 0; i < 16; ++i)
        obj->children[i] = nullptr;
    obj->field_0x18 = 0;
    obj->field_0x28 = UnkFlagsNone_7100161b90();
    obj->field_0x2c = 0;
    obj->field_0x30 = 0;
    u32 last = --unk_716e2cadf8;
    UnkObj_7100161b90* moved = unk_716e2aadf8[last];
    unk_716e2aadf8[index] = moved;
    moved->index = index;
    unk_716e2aadf8[last] = obj;
    obj->index = 0xffffffff;
}
