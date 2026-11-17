// Unit 182 (the device layer's texture records, as src/engine/UnkGfxSamplerSlots.cpp): creates a
// format 0x37 texture of the given size in the next free record, its rows padded to the device's
// alignment (device info 0x38), and registers it with a free descriptor id. Names and layouts are
// placeholders.
#include "game/UnkTU172.h"
#include "game/UnkTU176.h"
#include "game/UnkTU182.h"
#include "types.h"

struct UnkRenderTargetSlot;
struct UnkObject_0x130;

// A texture record: its size, kind and NVN format, the NVN texture object, its descriptor id and
// the memory it was placed in.
struct UnkGfxTexture {
    s32 width = 0;
    s32 height = 0;
    s32 kind = -1;     // 0x8
    s32 format;        // 0xc
    s32 field_0x10;
    u8 pad_0x14[0x4];
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

extern UnkGfxTexture* g_unk_716e2a3568[/* 0x800; unsized: x86-64 would assume 16-byte alignment */];
extern u32 g_unk_716e2a7568;   // live records
extern u32 g_unk_7101a1d814;   // free texture descriptor ids (2048 at start)
extern s32 g_unk_716e2a7708[/* 0x800; unsized: x86-64 would assume 16-byte alignment */];  // free texture descriptor ids, a stack
extern u8 g_unk_716e2a76e8[];  // the texture pool
extern UnkObject_0x130* g_unk_716e2a76a0;

void* fn_7100153a10(UnkGfxHeap* heap, void* old, u64 size, u64 align);

extern void (*g_unk_716e29b0d0)(void* builder, s32 value);
extern void (*g_unk_716e29b0d8)(void* builder, s32 value);
extern void (*g_unk_716e29b118)(void* builder, s32 format);
extern void (*g_unk_716e29b100)(void* builder, s32 width, s32 height);
extern void (*g_unk_716e29b160)(void* builder, s64 stride);
extern u64 (*g_unk_716e29b140)(void* builder);  // storage alignment
extern u64 (*g_unk_716e29b138)(void* builder);  // storage size
extern void (*g_unk_716e29b148)(void* builder, UnkGfxHeap* heap, u64 offset);
extern u8 (*g_unk_716e29b288)(void* texture, void* builder);  // an NVN boolean
extern void (*g_unk_716e29afc8)(void* pool, s32 id, void* texture, void* view);

// Through a wrapper: the arguments are evaluated before the entry point is loaded.
template <class R, class... A, class... B>
static inline R CallNvn(R (*&fn)(A...), B... args) {
    return fn(args...);
}

// The heap this kind of texture is placed in.
static inline UnkGfxHeap* BufferHeap() {
    return reinterpret_cast<UnkGfxHeap*>(g_unk_716e2a76a0);
}

// `value` rounded up to a multiple of `align`.
static inline u32 AlignUp(u32 value, u32 align) {
    return value + (align - value % align) % align;
}

void fn_710015fff0(UnkRenderTargetSlot* out, u32 width, u32 height) {
    UnkGfxTexture** result = reinterpret_cast<UnkGfxTexture**>(out);
    *result = nullptr;
    u32 n = g_unk_716e2a7568;
    if (n >= 0x800)
        return;
    UnkGfxTexture* t = g_unk_716e2a3568[n];
    t->kind = 0;
    t->format = 0x37;
    t->width = width;
    t->height = height;
    t->storage = nullptr;
    t->size = 0;
    t->field_0x10 = 1;
    t->field_0xf0 = 1;
    CallNvn(g_unk_716e29b0c8, fn_7100153df0());
    CallNvn(g_unk_716e29b0d8, fn_7100153df0(), t->field_0x10);
    CallNvn(g_unk_716e29b0d0, fn_7100153df0(), 0x10);
    CallNvn(g_unk_716e29b118, fn_7100153df0(), t->format);
    CallNvn(g_unk_716e29b100, fn_7100153df0(), t->width, t->height);
    CallNvn(g_unk_716e29b0d0, fn_7100153df0(), 2);
    s32 align_rows;
    CallNvn(g_unk_716e29ac78, fn_7100153cd0(), 0x38, &align_rows);
    s32 stride = AlignUp(AlignUp(width, align_rows) * 4, align_rows);
    CallNvn(g_unk_716e29b160, fn_7100153df0(), (s64)stride);
    u64 align = CallNvn(g_unk_716e29b140, fn_7100153df0());
    u64 size = CallNvn(g_unk_716e29b138, fn_7100153df0());
    u64 total = size + align;
    t->new_storage = fn_7100153a10(BufferHeap(), nullptr, total, align);
    u64 offset = reinterpret_cast<u64>(t->new_storage) - BufferHeap()->base + align;
    CallNvn(g_unk_716e29b148, fn_7100153df0(), BufferHeap(), offset);
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
    t->index = g_unk_716e2a7568;
    ++g_unk_716e2a7568;
    *result = t;
}
