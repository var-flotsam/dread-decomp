// The device layer's texture and sampler records (tools/tu_map.py unit 182, 0x710015f100-
// 0x7100162a48): a pool of 2048 texture records with their descriptor ids, the texture and sampler
// pools, and three groups of 16 sampler slots (include/game/UnkGraphicsState.h's g_unk_716e2a9730)
// whose setters keep the requested value, map it to the NVN value and mark the slot dirty.
// fn_710015f100 and fn_7100161520 run during the device set-up (fn_71001587f0). Names and layouts
// are placeholders.
#include "game/UnkGraphicsState.h"
#include "base/UnkVector.h"
#include "types.h"

// The default allocator (base/UnkDefaultAllocator.h) up to its slot 0x90: the free size, summed over
// its sub-allocators (as in src/engine/UnkGfxBlockPool.cpp).
struct UnkGfxTextureAllocator {
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

static inline UnkGfxTextureAllocator* GfxTextureAllocator() {
    return reinterpret_cast<UnkGfxTextureAllocator*>(fn_7100083990());
}

// A texture record: its size, kind and NVN format, the NVN texture object, its descriptor id and
// the memory it was placed in.
struct UnkGfxTexture {
    s32 width = 0;
    s32 height = 0;
    s32 kind = -1;     // 0x8
    s32 format;        // 0xc
    u8 pad_0x10[0x8];
    u8 texture[0xc0];  // 0x18: the NVN texture
    s32 id = 0;               // 0xd8: texture descriptor id
    u8 pad_0xdc[0x4];
    void* storage = nullptr;  // 0xe0
    u32 size = 0;             // 0xe8: of the storage
    s32 index = -1;           // 0xec: in g_unk_716e2a3568
    s32 field_0xf0 = 0;
    u8 pad_0xf4[0x4];
    void* new_storage = nullptr;  // 0xf8
};
static_assert(sizeof(UnkGfxTexture) == 0x100, "UnkGfxTexture layout");

// A heap of texture memory: its NVN memory pool first, the pool's base address at +0x100.
struct UnkGfxHeap {
    u8 pool[0x100];
    u64 base;  // 0x100
};

struct UnkPool;  // the memory pool type of src/game_s7/fn_71001587f0.cpp: its NVN pool at +0x8

// The records and the descriptor pools (all reached through the GOT).
extern UnkGfxTexture* g_unk_716e2a3568[0x800];
extern u32 g_unk_716e2a7568;   // live records
extern UnkGfxHeap* g_unk_716e2a76a8;  // the heap for textures
extern UnkGfxHeap* g_unk_716e2a76b0;  // the heap for render targets
extern u32 g_unk_7101a1d814;   // free texture descriptor ids (2048 at start)
extern s32 g_unk_716e2a7574;   // reserved texture descriptors (device info 0x11)
extern s32 g_unk_716e2a7578;   // reserved sampler descriptors (device info 0x12)
extern s32 g_unk_716e2a7708[0x800];  // free texture descriptor ids, a stack
extern s32 g_unk_716e2a756c;   // the next free sampler id
extern s32 g_unk_716e2a7570;   // the last sampler id
extern s32 g_unk_716e2a76e0;   // the first sampler id (the reserved ones come before it)
extern UnkPool g_unk_716e2a7580;  // the descriptors' memory
extern u8 g_unk_716e2a76b8[];  // the sampler pool
extern u8 g_unk_716e2a76e8[];  // the texture pool

void* fn_7100153cd0();  // the device
void* fn_7100153de0();
void* fn_7100153df0();  // the texture builder
void fn_71001604e0(void* builder, s32 format);
// Allocates `size` bytes of a heap at an alignment, copying the old block's bytes when there is one.
void* fn_7100153a10(UnkGfxHeap* heap, void* old, u64 size, u64 align);
void fn_710015385c(UnkPool* pool, u64 size, s32 flags, void* device);
// Reserves `size` bytes of a pool at the given alignment; returns their offset (declared without
// its result in src/game_s7/fn_71001587f0.cpp).
void fn_7100153968(UnkPool* pool, u64 size, u64 align);

// NVN entry points (the device info query as src/game_s7/fn_71001587f0.cpp declares it).
extern void (*g_unk_716e29ac78)(void* device, s32 name, s32* value);
extern void (*g_unk_716e29aff0)(void* pool, void* memory, u64 offset, s32 count);  // sampler pool
extern void (*g_unk_716e29afb0)(void* pool, void* memory, u64 offset, s32 count);  // texture pool
extern void (*g_unk_716e29b380)(void* a, void* device);
extern void (*g_unk_716e29b0c8)(void* builder);  // as src/game_s7/fn_71001587f0.cpp declares it
extern void (*g_unk_716e29b0d0)(void* builder, s32 value);
extern void (*g_unk_716e29b0d8)(void* builder, s32 value);
extern void (*g_unk_716e29b118)(void* builder, s32 format);
extern void (*g_unk_716e29b100)(void* builder, s32 width, s32 height);
extern u64 (*g_unk_716e29b140)(void* builder);  // storage alignment
extern u64 (*g_unk_716e29b138)(void* builder);  // storage size
extern void (*g_unk_716e29b148)(void* builder, UnkGfxHeap* heap, u64 offset);
extern u8 (*g_unk_716e29b288)(void* texture, void* builder);  // an NVN boolean
extern void (*g_unk_716e29afc8)(void* pool, s32 id, void* texture, void* view);

// Through a wrapper: the arguments are evaluated before the entry point is loaded.
template <class R, class... A, class... B>
__attribute__((always_inline)) static inline R CallNvn(R (*&fn)(A...), B... args) {
    return fn(args...);
}

static inline u64 PoolReserve(UnkPool* pool, u64 size, u64 align) {
    return reinterpret_cast<u64 (*)(UnkPool*, u64, u64)>(fn_7100153968)(pool, size, align);
}

// A sampler slot (include/game/UnkGraphicsState.h's UnkGraphicsSlot) with the fields this unit
// sets: the requested modes and the NVN values they map to.
struct UnkGfxSamplerSlot {
    void* ptr;
    u8 pad_0x8[0x8];
    s32 state;        // 0x10
    s32 field_0x14;
    s32 field_0x18;
    s32 transition;   // 0x1c
    s32 field_0x20;   // field_0x18 mapped
    s32 wrap[3];      // 0x24
    s32 field_0x30;
    s32 field_0x34;
    s32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    s32 nvn_wrap[3];  // 0x48: wrap mapped
    s32 field_0x54;
    s32 field_0x58;
    s32 field_0x5c;
    s32 field_0x60;
    s32 field_0x64;
    u64 field_0x68;
    bool dirty;       // 0x70
    bool field_0x71;
    u8 pad_0x72[0x6];
};
static_assert(sizeof(UnkGfxSamplerSlot) == sizeof(UnkGraphicsSlot), "sampler slot size");

static inline UnkGfxSamplerSlot& Slot(u32 group, u32 index) {
    return reinterpret_cast<UnkGfxSamplerSlot&>(g_unk_716e2a9730[group][index]);
}

void* fn_7100153d50();  // the context the unbind callback takes

// A slot's default state: unbound values, every mode at its default, marked dirty.
static inline void ResetSlot(UnkGfxSamplerSlot& s) {
    s.state = 2;
    s.field_0x14 = 1;
    s.field_0x18 = 1;
    s.wrap[0] = 2;
    s.wrap[1] = 2;
    s.wrap[2] = 2;
    s.field_0x30 = 1;
    s.field_0x34 = 0;
    s.field_0x38 = 0;
    s.transition = 3;
    s.field_0x20 = 0;
    s.nvn_wrap[0] = 1;
    s.nvn_wrap[1] = 1;
    s.nvn_wrap[2] = 1;
    s.field_0x54 = 0;
    s.field_0x58 = 0;
    s.field_0x5c = 0;
    s.field_0x60 = 0;
    s.field_0x3c = -16.0f;
    s.field_0x40 = 16.0f;
    s.field_0x44 = 15.0f;
    s.field_0x64 = 0;
    s.field_0x68 = 0;
    s.dirty = true;
    s.field_0x71 = false;
}

// Resets the 16 slots of a group; a bound one is unbound and the unbinding reported with the
// group's id.
static inline void ResetGroup(u32 group) {
    for (u32 i = 0; i < 16; ++i) {
        ResetSlot(Slot(group, i));
        if (Slot(group, i).ptr) {
            Slot(group, i).ptr = nullptr;
            void* context = fn_7100153d50();
            u32 id = g_unk_7101a1d818[group];
            g_unk_716e29b858(context, id, i, nullptr);
        }
    }
}

// Creates the texture records, the free descriptor ids and the texture and sampler pools (as many
// descriptors as the device allows, up to 2048 textures and 8192 samplers past the reserved ones),
// then resets the sampler slots. The free size is read before and after the records (unused).
// NON_MATCHING(minor): in the second and third slot loops two address computations (+0x3c and
// +0x54 of the slot) are scheduled and allocated the other way round; the stores are the same.
void fn_710015f100() {
    GfxTextureAllocator()->vf_0x90();
    for (UnkGfxTexture*& t : g_unk_716e2a3568)
        t = new (fn_71000839a0(sizeof(UnkGfxTexture), 0x46)) UnkGfxTexture;
    g_unk_716e2a7568 = 0;
    GfxTextureAllocator()->vf_0x90();
    s32 texture_size = 0;
    s32 sampler_size = 0;
    s32 max_textures = 0;
    s32 max_samplers = 0;
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 0xf, &texture_size);
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 0x10, &sampler_size);
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 0x11, &g_unk_716e2a7574);
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 0x24, &max_textures);
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 0x12, &g_unk_716e2a7578);
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 0x25, &max_samplers);
    s32 textures = g_unk_716e2a7574 + 0x800;
    if (max_textures < textures)
        textures = max_textures;
    s32 samplers = g_unk_716e2a7578 + 0x2000;
    if (max_samplers < samplers)
        samplers = max_samplers;
    for (s32 i = 0; i < 0x800; ++i)
        g_unk_716e2a7708[i] = g_unk_716e2a7574 + 0x7ff - i;
    g_unk_716e2a756c = g_unk_716e2a7578;
    g_unk_716e2a7570 = max_samplers - 1;
    u32 size = sampler_size * samplers + texture_size * textures;
    fn_710015385c(&g_unk_716e2a7580, size, 0x22, fn_7100153cd0());
    void* memory = reinterpret_cast<u8*>(&g_unk_716e2a7580) + 8;
    u64 offset = PoolReserve(&g_unk_716e2a7580, (s64)sampler_size * samplers, 8);
    CallNvn(g_unk_716e29aff0, g_unk_716e2a76b8, memory, offset, samplers);
    offset = PoolReserve(&g_unk_716e2a7580, (s64)texture_size * textures, 8);
    CallNvn(g_unk_716e29afb0, g_unk_716e2a76e8, memory, offset, textures);
    g_unk_716e2a76e0 = g_unk_716e2a7578;
    CallNvn(g_unk_716e29b380, fn_7100153de0(), fn_7100153cd0());
    for (u32 i = 0; i < 16; ++i)
        ResetSlot(Slot(0, i));
    for (u32 i = 0; i < 16; ++i)
        ResetSlot(Slot(1, i));
    for (u32 i = 0; i < 16; ++i)
        ResetSlot(Slot(2, i));
}

// NON_MATCHING(minor): in the second and third group loops two address computations (+0x3c and
// +0x54 of the slot) are scheduled and allocated the other way round; the stores are the same.
void fn_710015f500() {
    ResetGroup(0);
    ResetGroup(1);
    ResetGroup(2);
}

// Sets wrap mode `axis` of a slot: the requested mode, NVN's for it (clamp to edge, clamp to
// border, repeat, mirrored repeat; another mode keeps the last one) and the dirty flag.
static inline void SetWrapMode(u32 index, s32 mode, u32 group, s32 axis) {
    if (index > 15)
        return;
    if (Slot(group, index).wrap[axis] == mode)
        return;
    Slot(group, index).wrap[axis] = mode;
    switch (mode) {
    case 0:
        Slot(group, index).nvn_wrap[axis] = 7;
        break;
    case 1:
        Slot(group, index).nvn_wrap[axis] = 5;
        break;
    case 2:
        Slot(group, index).nvn_wrap[axis] = 1;
        break;
    case 3:
        Slot(group, index).nvn_wrap[axis] = 6;
        break;
    }
    Slot(group, index).dirty = true;
}

void fn_7100160810(u32 index, s32 mode, u32 group) {
    SetWrapMode(index, mode, group, 0);
}

void fn_7100160890(u32 index, s32 mode, u32 group) {
    SetWrapMode(index, mode, group, 1);
}

void fn_7100160910(u32 index, s32 mode, u32 group) {
    SetWrapMode(index, mode, group, 2);
}

// Sets a slot's mode at +0x18 and NVN's value for it (modes 0-1 give 0, 2-3 give 1; another mode
// keeps the last one).
void fn_7100160ab0(u32 index, s32 mode, u32 group) {
    if (index > 15)
        return;
    if (Slot(group, index).field_0x18 == mode)
        return;
    Slot(group, index).field_0x18 = mode;
    switch (mode) {
    case 0:
    case 1:
        Slot(group, index).field_0x20 = 0;
        break;
    case 2:
    case 3:
        Slot(group, index).field_0x20 = 1;
        break;
    }
    Slot(group, index).dirty = true;
}

// The counterpart of fn_7100160990 (src/game_lo/): sets the value at +0x14 and records the kind of
// transition from the state's group ({0,1} or {2,3}) to the new value's.
void fn_7100160d50(u32 index, s32 value, u32 group) {
    if (index > 15)
        return;
    const s32& state = Slot(group, index).state;
    if (Slot(group, index).field_0x14 == value)
        return;
    Slot(group, index).field_0x14 = value;
    switch (value) {
    case 0:
    case 1:
        switch (state) {
        case 0:
        case 1:
            Slot(group, index).transition = 2;
            break;
        case 2:
        case 3:
            Slot(group, index).transition = 3;
            break;
        }
        break;
    case 2:
    case 3:
        switch (state) {
        case 0:
        case 1:
            Slot(group, index).transition = 4;
            break;
        case 2:
        case 3:
            Slot(group, index).transition = 5;
            break;
        }
        break;
    }
    Slot(group, index).dirty = true;
}

// NVN's format for one of the engine's (0-16; another is RGBA8, 0x25).
static inline s32 NvnFormat(s32 format) {
    switch (format) {
    case 0:
        return 1;
    case 1:
        return 0xd;
    case 2:
        return 0x19;
    case 5:
        return 0xa;
    case 8:
        return 0x2a;
    case 9:
        return 0x29;
    case 10:
        return 0x2e;
    case 11:
        return 0x3f;
    case 12:
        return 0x32;
    case 13:
        return 0x33;
    case 14:
        return 0x35;
    case 15:
        return 0x34;
    case 16:
        return 0x36;
    default:
        return 0x25;
    }
}

// Creates a texture in the next free record: sets up the builder, places the storage in the heap
// for its kind, initialises the NVN texture and registers it with a free descriptor id. `out` is
// null when there is no free record or the texture could not be initialised.
// NON_MATCHING(minor): the heap is chosen with a csel of both GOT addresses after a 32-bit load of
// the flags (the original reloads the flags byte and branches to one GOT load); same heap.
void fn_7100160f60(UnkGfxTexture** out, s32 width, s32 height, s32 format, const u32& flags) {
    *out = nullptr;
    u32 n = g_unk_716e2a7568;
    if (n >= 0x800)
        return;
    UnkGfxTexture* t = g_unk_716e2a3568[n];
    s32 kind = (flags & 8) ? 7 : 1;
    t->kind = kind;
    t->storage = nullptr;
    t->width = width;
    t->size = 0;
    t->index = n;
    t->field_0xf0 = 1;
    t->height = height;
    t->format = NvnFormat(format);
    u32 bits = flags;
    s32 usage = 0xc;
    if (bits & 0x20)
        usage = 0xd;
    if (bits & 8)
        usage = 0x10;
    CallNvn(g_unk_716e29b0c8, fn_7100153df0());
    if (!(flags & 8))
        CallNvn(g_unk_716e29b0d0, fn_7100153df0(), usage);
    CallNvn(g_unk_716e29b0d8, fn_7100153df0(), 1);
    CallNvn(g_unk_716e29b118, fn_7100153df0(), t->format);
    CallNvn(g_unk_716e29b100, fn_7100153df0(), width, height);
    fn_71001604e0(fn_7100153df0(), t->format);
    u64 align = CallNvn(g_unk_716e29b140, fn_7100153df0());
    u64 size = CallNvn(g_unk_716e29b138, fn_7100153df0());
    UnkGfxHeap** heap = &g_unk_716e2a76a8;
    if (flags & 8)
        heap = &g_unk_716e2a76b0;
    u32 total = size + align;
    t->new_storage = fn_7100153a10(*heap, t->storage, total, align);
    u64 offset = reinterpret_cast<u64>(t->new_storage) - (*heap)->base + align;
    CallNvn(g_unk_716e29b148, fn_7100153df0(), *heap, offset);
    if (!CallNvn(g_unk_716e29b288, t->texture, fn_7100153df0()))
        return;
    s32 id;
    if (g_unk_7101a1d814 == 0) {
        id = 0;
    } else {
        --g_unk_7101a1d814;
        id = g_unk_716e2a7708[g_unk_7101a1d814];
    }
    t->id = id;
    CallNvn(g_unk_716e29afc8, g_unk_716e2a76e8, id, t->texture, nullptr);
    t->storage = t->new_storage;
    t->size = total;
    ++g_unk_716e2a7568;
    *out = t;
}

// The pools of src/game_lo/fn_7100161b90.cpp, which returns an object to them: 0x4000 objects of
// 0x1a8 bytes (their constructor is fn_7100161654) and 0x10000 child records, each with a table of
// pointers whose first entries (as many as the count) are the live ones.
struct UnkObj_7100161b90;
struct UnkChild_7100161b90;
extern UnkObj_7100161b90* unk_716e2aadf8[0x4000];  // as src/game_lo/fn_7100161b90.cpp declares them
extern u32 unk_716e2cadf8;
extern UnkChild_7100161b90* unk_716e2cae08[];
extern u32 unk_716e34ae08;

struct UnkGfxObject {
    u8 unk_0x0[0x1a8];
};

// A child record as created: unused, with the kind 0x16 that fn_7100161b90 also resets it to.
struct UnkGfxChild {
    s32 field_0x0 = -1;
    u32 field_0x4;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
    u32 field_0x18 = 0;
    u32 field_0x1c = 0x16;
    u64 field_0x20 = 0;
    u32 field_0x28;
    s32 index = -1;  // in unk_716e2cae08
};
static_assert(sizeof(UnkGfxChild) == 0x30, "UnkGfxChild layout");

extern UnkGfxObject* g_unk_716e2aadf0;  // the objects
extern UnkGfxChild* g_unk_716e2cae00;   // the child records

void fn_7100161654(UnkGfxObject* obj);  // constructs an object

// Creates both pools, every table entry pointing at its record and none live. The free size is read
// before, between and after (the results are unused).
// NON_MATCHING(minor): the two constants of the child records' loop (-1 and 0x1600000000) are
// materialised into each other's registers; the stores are the same.
void fn_7100161520() {
    GfxTextureAllocator()->vf_0x90();
    auto* objects = static_cast<UnkGfxObject*>(fn_7100083ba0(0x4000 * sizeof(UnkGfxObject), 0x44));
    for (u32 i = 0; i < 0x4000; ++i)
        fn_7100161654(&objects[i]);
    g_unk_716e2aadf0 = objects;
    for (u32 i = 0; i < 0x4000; ++i)
        unk_716e2aadf8[i] = reinterpret_cast<UnkObj_7100161b90*>(&objects[i]);
    unk_716e2cadf8 = 0;
    GfxTextureAllocator()->vf_0x90();
    auto* children = new (fn_7100083ba0(0x10000 * sizeof(UnkGfxChild), 0x44)) UnkGfxChild[0x10000];
    g_unk_716e2cae00 = children;
    for (u32 i = 0; i < 0x10000; ++i)
        unk_716e2cae08[i] = reinterpret_cast<UnkChild_7100161b90*>(&children[i]);
    unk_716e34ae08 = 0;
    GfxTextureAllocator()->vf_0x90();
}
