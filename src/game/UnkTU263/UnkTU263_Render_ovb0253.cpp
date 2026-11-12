// Unit 263 (render passes, no reliable name): the texel upload and passes of the owner whose graphics target is at +0xa8;
// each hands the target a global string id, binds a list and a block, sets the viewport from the
// screen size, and runs a fixed run of its methods. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU177.h"
#include "game/UnkTU179.h"
#include "game/UnkTU259.h"
#include "types.h"

using base::global::CStrId;

struct UnkRenderTargetA1;

extern CStrId g_unk_716e596680 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596688 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596690 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e596698 __attribute__((visibility("hidden")));
extern f32 g_unk_716e596650[3] __attribute__((visibility("hidden")));
extern u32 g_unk_7101a1e9bc;
extern u32 g_unk_7101a1e9c0;

// Takes a mode number in the first slot (the declaration elsewhere calls it with true).
struct UnkRenderCalls_b0253 {
    static void fn_71001cde84(UnkRenderTargetA1* target, u32 a, bool b, bool c);
};
void fn_71001d1ec8(UnkRenderTargetA1* target, f32 value);
void fn_71001d1ed0(UnkRenderTargetA1* target, u32 value);
void fn_71001cf5e4(UnkRenderTargetA1* target, s32 mode);
struct UnkBoundTarget;
struct UnkTexelTarget;
void fn_7100160e60(void* target);
void fn_7100160e70(void* target);
void fn_7100160e80(UnkTexelTarget* target, u32 x, u32 y, u32 value);

struct UnkPassOwner_b0253 {
    u8 pad_0x0[0x8];
    void* list_0x8;  // 0x8
    u32 count_0x10;  // 0x10
    u8 pad_0x14[0x1c];
    void* list_0x30;  // 0x30
    u32 count_0x38;  // 0x38
    u8 pad_0x3c[0x1c];
    void* list_0x58;  // 0x58
    u32 count_0x60;  // 0x60
    u8 pad_0x64[0x1c];
    void* list_0x80;  // 0x80
    u32 count_0x88;  // 0x88
    u8 pad_0x8c[0x1c];
    UnkRenderTargetA1* target;  // 0xa8
    u8 pad_0xb0[0x8];
    u8 viewport_0xb8[0x18];  // 0xb8
    u8 block_0xd0[0xd8];     // 0xd0
    u8 block_0x1a8[0x60];    // 0x1a8
    u8 block_0x208[0x98];    // 0x208
    u8 block_0x2a0[0x98];    // 0x2a0
    u8 block_0x338[0x98];    // 0x338
};
static_assert(__builtin_offsetof(UnkPassOwner_b0253, target) == 0xa8, "UnkPassOwner_b0253 layout");
static_assert(__builtin_offsetof(UnkPassOwner_b0253, block_0x338) == 0x338, "UnkPassOwner_b0253 layout");

// Sets the viewport around the screen-sized area with a margin.
#define SET_VIEWPORT()                                                                   \
    do {                                                                                 \
        u32 w = g_unk_7101a1e9bc;                                                        \
        u32 h = g_unk_7101a1e9c0;                                                        \
        fn_710015a090((0x500 - w) / 2 - 2, (0x2d0 - h) / 2 - 2, w + 4, h + 4);          \
    } while (0)

void fn_71001e2518(UnkPassOwner_b0253* self) {
    fn_71001cdd6c(self->target, g_unk_716e596698);
    fn_71001d0b70(self->target, self->list_0x80, self->count_0x88, -1, -1);
    fn_71001cdba4(self->target, self->block_0x338);
    fn_71001cdd74(self->target, self->viewport_0xb8);
    SET_VIEWPORT();
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001ced08(self->target, 0);
    UnkRenderCalls_b0253::fn_71001cde84(self->target, 1, true, true);
    UnkRenderCalls_b0253::fn_71001cde84(self->target, 0x10, true, true);
    UnkRenderCalls_b0253::fn_71001cde84(self->target, 0xe, true, true);
    fn_71001cde48(self->target);
    fn_71001cdd18(self->target);
    fn_71001cdd70(self->target);
}

void fn_71001e2184(UnkPassOwner_b0253* self) {
    fn_71001cdd6c(self->target, g_unk_716e596690);
    fn_71001d0b70(self->target, self->list_0x58, self->count_0x60, -1, -1);
    fn_71001cdba4(self->target, self->block_0x2a0);
    fn_71001cdd74(self->target, self->viewport_0xb8);
    SET_VIEWPORT();
    fn_71001d1e78(self->target, 0.0f, 0.0f, 0.0f, 0.0f);
    fn_71001d1ec8(self->target, 1.0f);
    fn_71001d1ed0(self->target, 0);
    fn_71001cc550(self->target, 1, 1, 0);
    fn_71001cc570(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001ced08(self->target, 0);
    UnkRenderCalls_b0253::fn_71001cde84(self->target, 0x1f, true, true);
    fn_71001cde48(self->target);
    fn_71001cdd18(self->target);
    fn_71001cdd70(self->target);
}

void fn_71001e1788(UnkPassOwner_b0253* self) {
    fn_71001cdd6c(self->target, g_unk_716e596688);
    fn_71001d0b70(self->target, self->list_0x8, self->count_0x10, -1, -1);
    fn_71001cdba4(self->target, self->block_0xd0);
    fn_71001cdd74(self->target, self->viewport_0xb8);
    SET_VIEWPORT();
    fn_71001d1e78(self->target, 0.0f, 0.0f, 0.0f, 0.0f);
    fn_71001d1ec8(self->target, 1.0f);
    fn_71001d1ed0(self->target, 0);
    fn_71001cc550(self->target, 1, 1, 0);
    fn_71001cc570(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001ced08(self->target, 0);
    fn_71001ce704(self->target, 0x10);
    fn_71001cf5e4(self->target, 0);
    fn_71001cde48(self->target);
    fn_71001cdd18(self->target);
    fn_71001cdd70(self->target);
}

void fn_71001e2334(UnkPassOwner_b0253* self) {
    fn_71001cdd6c(self->target, g_unk_716e596680);
    fn_71001d0b70(self->target, self->list_0x30, self->count_0x38, -1, -1);
    fn_71001cdba4(self->target, self->block_0x208);
    fn_71001cdd74(self->target, self->viewport_0xb8);
    SET_VIEWPORT();
    fn_71001d1e78(self->target, g_unk_716e596650[0], g_unk_716e596650[1], g_unk_716e596650[2], 0.3f);
    fn_71001d1ec8(self->target, 1.0f);
    fn_71001d1ed0(self->target, 0);
    fn_71001cc550(self->target, 1, 1, 0);
    fn_71001cc570(self->target);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_71001ced08(self->target, 0);
    UnkRenderCalls_b0253::fn_71001cde84(self->target, 1, true, true);
    UnkRenderCalls_b0253::fn_71001cde84(self->target, 0x10, true, true);
    UnkRenderCalls_b0253::fn_71001cde84(self->target, 0xe, true, true);
    fn_71001cde48(self->target);
    fn_71001cdd18(self->target);
    fn_71001cdd70(self->target);
}

// The pending texel upload: the flag, the rectangle to write and the pixels (1024 per row).
struct UnkUploadState_b0253 {
    u8 pending;  // 0x638
    u8 pad_0x1[7];
    u32 x_begin;  // 0x640
    u32 x_end;    // 0x644
    u32 y_begin;  // 0x648
    u32 y_end;    // 0x64c
    u8 pad_0x650[0x60];
    u32* pixels;  // 0x6b0
};
static_assert(__builtin_offsetof(UnkUploadState_b0253, pixels) == 0x78, "UnkUploadState_b0253 layout");
extern UnkUploadState_b0253 g_unk_716e596638 __attribute__((visibility("hidden")));

struct UnkUploadOwner_b0253 {
    u8 pad_0x0[0x200];
    UnkBoundTarget* target;  // 0x200
};

void fn_71001e1684(UnkUploadOwner_b0253* self) {
    UnkUploadState_b0253& up = g_unk_716e596638;
    if (up.pending != 1)
        return;
    if (up.pixels) {
        void* texture = fn_710015c450(self->target);
        fn_7100160e60(texture);
        for (u32 x = up.x_begin; x <= up.x_end; ++x) {
            for (u32 y = up.y_begin; y <= up.y_end; ++y)
                fn_7100160e80(static_cast<UnkTexelTarget*>(texture), x, y, up.pixels[x + (y << 10)]);
        }
        fn_7100160e70(texture);
        up.pixels = nullptr;
    }
    up.pending = 0;
}
