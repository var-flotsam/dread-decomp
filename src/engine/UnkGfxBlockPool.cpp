// The device layer's pool of memory-block records (tools/tu_map.py unit 174, 0x7100153f40-
// 0x7100154538): 0x4000 records of 0x68 bytes in one allocation, and a table of pointers to them
// whose first g_unk_716e29ab40 entries are the live ones (src/game_lo/fn_71001541a0.cpp returns a
// record to the pool). fn_7100153f40 creates the pool during the device set-up (fn_71001587f0).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// A record: only its flags are cleared here (bit 0 says which heap its block came from).
struct UnkGfxBlock {
    u8 unk_0x0[0x1c];
    u32 flags;  // 0x1c
    u8 unk_0x20[0x48];
};
static_assert(sizeof(UnkGfxBlock) == 0x68, "UnkGfxBlock layout");

constexpr u32 kGfxBlockCount = 0x4000;

// The record as src/game_lo/fn_71001541a0.cpp sees it, and the globals both files use.
struct UnkObj_71001541a0;
extern u32 g_unk_716e29ab40;                   // live count
extern UnkObj_71001541a0* g_unk_716e27ab38[];  // the table
extern UnkGfxBlock* g_unk_716e29ab38;          // the records

// The default allocator (base/UnkDefaultAllocator.h) up to its slot 0x90: the free size, summed over
// its sub-allocators (fn_71000834b8).
struct UnkGfxPoolAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual u64 vf_0x90();
};

static inline UnkGfxPoolAllocator* GfxPoolAllocator() {
    return reinterpret_cast<UnkGfxPoolAllocator*>(fn_7100083990());
}

// Creates the pool: every record cleared, every table entry pointing at its record, none live. The
// free size is read before and after (the results are unused).
void fn_7100153f40() {
    GfxPoolAllocator()->vf_0x90();
    auto* blocks = static_cast<UnkGfxBlock*>(fn_7100083ba0(kGfxBlockCount * sizeof(UnkGfxBlock), 0x45));
    for (UnkGfxBlock *p = blocks, *end = blocks + kGfxBlockCount; p != end; ++p)
        p->flags = 0;
    g_unk_716e29ab38 = blocks;
    for (u32 i = 0; i < kGfxBlockCount; ++i)
        g_unk_716e27ab38[i] = reinterpret_cast<UnkObj_71001541a0*>(&blocks[i]);
    g_unk_716e29ab40 = 0;
    GfxPoolAllocator()->vf_0x90();
}
