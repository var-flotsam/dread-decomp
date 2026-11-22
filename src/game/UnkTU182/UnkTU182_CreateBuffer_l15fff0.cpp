// Unit 182 (the device layer's texture records, as src/engine/UnkGfxSamplerSlots.cpp): creates a
// format 0x37 texture of the given size in the next free record, its rows padded to the device's
// alignment (device info 0x38), and registers it with a free descriptor id. Names and layouts are
// placeholders.
#include "engine/UnkGfxTexture.h"
#include "game/UnkGfxHeap.h"
#include "game/UnkTU170.h"
#include "game/UnkTU172.h"
#include "game/UnkTU176.h"
#include "game/UnkTU182.h"
#include "types.h"

struct UnkRenderTargetSlot;
struct UnkObject_0x130;

static_assert(sizeof(UnkGfxTexture) == 0x100, "UnkGfxTexture layout");

extern void (*g_unk_716e29b160)(void* builder, s64 stride);

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
