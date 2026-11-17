// Unit 263 (minimap rendering): sets one of the unit's render parameters, chosen by the hash of
// its name, from the value `value` points to: colours, the screen setup, the texel buffer, or a
// region of it to upload (merged into the pending one). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrIdBase;

struct UnkStatics_716e596638 {
    bool pending;  // a region waits to be uploaded
    u8 pad_0x1[0x3];
    u32 field_0x4;
    u32 x_begin;  // 0x8
    u32 x_end;
    u32 y_begin;
    u32 y_end;
    f32 field_0x18[3];
    u8 pad_0x24[0x4];
    UnkStrIdBase binaries;
    UnkStrIdBase sources;
    UnkStrIdBase minimap_pass;
    UnkStrIdBase minimap_mask_pass;
    UnkStrIdBase minimap_zone_pass;
    UnkStrIdBase minimap_color_pass;
    UnkStrIdBase minimap_maproom_pass;
    UnkStrIdBase minimap_2d_pass;
    UnkStrIdBase minimap_filter_pass;
    UnkStrIdBase minimap;
    u32* pixels;  // 0x78
};
static_assert(__builtin_offsetof(UnkStatics_716e596638, pixels) == 0x78, "UnkStatics_716e596638 layout");
extern UnkStatics_716e596638 g_unk_716e596638 __attribute__((visibility("hidden")));

extern f32 g_unk_716e596768[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1ea70[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1ea7c[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1ea88[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1ea94[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1eaa0[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1eaac[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1eab8[3] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1eac4[4] __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1ead4[3] __attribute__((visibility("hidden")));
extern u32 g_unk_7101a1ea68[2] __attribute__((visibility("hidden")));
extern u32 g_unk_7101a1e9bc;
extern u32 g_unk_7101a1e9c0;

// The screen setup: its size in floats, and two more words.
struct UnkScreenSetup {
    f32 width;
    f32 height;
    u32 field_0x8;
    u32 field_0xc;
};

// A texel region, inclusive.
struct UnkRegion {
    u32 x_begin;
    u32 y_begin;
    u32 x_end;
    u32 y_end;
};

static inline void SetColor(f32* dst, const void* value) {
    const f32* src = static_cast<const f32*>(value);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

void fn_71001e2acc(void* self, u64 hash, void* value) {
    switch (hash) {
    case 0x7bc15ce967f525d2: {
        const UnkRegion* r = static_cast<const UnkRegion*>(value);
        if (r->x_begin > r->x_end || r->y_begin > r->y_end)
            break;
        UnkStatics_716e596638& s = g_unk_716e596638;
        if (s.pending) {
            s.x_begin = s.x_begin < r->x_begin ? s.x_begin : r->x_begin;
            s.y_begin = s.y_begin < r->y_begin ? s.y_begin : r->y_begin;
            s.x_end = s.x_end > r->x_end ? s.x_end : r->x_end;
            s.y_end = s.y_end > r->y_end ? s.y_end : r->y_end;
        } else {
            s.x_begin = r->x_begin;
            s.y_begin = r->y_begin;
            s.x_end = r->x_end;
            s.y_end = r->y_end;
        }
        s.pending = true;
        break;
    }
    case 0xfad8a189175fde2e: {
        const UnkScreenSetup* setup = static_cast<const UnkScreenSetup*>(value);
        u32 width = setup->width;
        u32 height = setup->height;
        u32 a = setup->field_0x8;
        u32 b = setup->field_0xc;
        g_unk_7101a1ea68[0] = a;
        g_unk_7101a1ea68[1] = b;
        g_unk_7101a1e9bc = width;
        g_unk_7101a1e9c0 = height;
        break;
    }
    case 0x02187b0175c717fb:
        SetColor(g_unk_7101a1ead4, value);
        break;
    case 0x6fc641f145352069:
        g_unk_716e596638.pixels = static_cast<u32*>(value);
        break;
    case 0x64a1192276c42eb8:
        SetColor(g_unk_7101a1eaac, value);
        break;
    case 0x8326972bd3d03767:
        SetColor(g_unk_716e596638.field_0x18, value);
        break;
    case 0xb94590d0d0d7a38e:
        SetColor(g_unk_716e596768, value);
        break;
    case 0xe134bd7d84d91c3b:
        SetColor(g_unk_7101a1eaa0, value);
        break;
    case 0x44213fb099a861a3:
        SetColor(g_unk_7101a1ea70, value);
        break;
    case 0x48620bbd87ab24c0:
        SetColor(g_unk_7101a1ea94, value);
        break;
    case 0x604391e752cd5cbd: {
        const f32* src = static_cast<const f32*>(value);
        g_unk_7101a1eac4[0] = src[0];
        g_unk_7101a1eac4[1] = src[1];
        g_unk_7101a1eac4[2] = src[2];
        g_unk_7101a1eac4[3] = src[3];
        break;
    }
    case 0x2333d73f2bb4f379:
        SetColor(g_unk_7101a1ea88, value);
        break;
    case 0x39e8a1b1791ff383:
        SetColor(g_unk_7101a1eab8, value);
        break;
    case 0x7f23f21cedda3536:
        SetColor(g_unk_7101a1ea7c, value);
        break;
    }
}
